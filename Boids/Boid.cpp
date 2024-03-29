#include "Boid.h"


#include <cmath>

Boid::Boid()
{
	isDead = false;
	m_flag = fishBoid;
	m_position = XMFLOAT3(0, 0, 0);
	m_direction = XMFLOAT3(0, 1, 0);
	createRandomDirection();
	randomizeStats();
	setScale(m_scale); 

}

Boid::~Boid()
{
}

void Boid::createRandomDirection()
{
	float x = (float)(rand() % 10);
	x -= 5;
	float y = (float)(rand() % 10);
	y -= 5;
	float z = 0;

	setDirection(XMFLOAT3(x, y, z));
}

void Boid::randomizeStats()
{
	m_speed = randomizeWithinFraction(FISH_SPEED, 0.05f);
	m_sightDistance = randomizeWithinFraction(BOID_SIGHT_RANGE, 0.5f);
	m_cohesionWeight = randomizeWithinFraction(COHESION_STRENGTH, 0.35f);
	m_cohesionMultiplier = randomizeWithinFraction(COHESION_MULTIPLIER, 0.2f);
	m_separationWeight = randomizeWithinFraction(SEPARATION_STRENGTH, 0.6f);
	m_separationDistance = randomizeWithinFraction(SEPARATION_DISTANCE, 0.5f);
	m_smallFlockNumber = (int)randomizeWithinFraction(SMALL_FLOCK_NUMBER, 0.8f);
	m_alignmentWeight = randomizeWithinFraction(ALIGNMENT_STRENGTH, 0.8f);
	m_fleeWeight = randomizeWithinFraction(FLEE_STRENGTH, 0.6f);
	m_sightDistance = randomizeWithinFraction(BOID_SIGHT_RANGE, 0.15f);
	m_sightArc = 100.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (150.0f - 100.0f)));
	m_scale = FISH_SCALE;

	m_turningDelta = randomizeWithinFraction(SMOOTH_VALUE_FISH, 0.15f);

}

float Boid::randomizeWithinFraction(double value, float fraction)
{
	double max = (value * (1.0f + fraction));
	double min = (value * (1.0f - fraction));
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void Boid::setDirection(XMFLOAT3 direction)
{
	XMVECTOR v = XMLoadFloat3(&direction);
	v = XMVector3Normalize(v);
	XMVECTOR vMagSq = XMVector3LengthSq(v);

	//direction cannot be 0. Check if the magnitude is greater than 0. If not, set direction to normalized X vector.
	if (vMagSq.m128_f32[0] > 0) { //check the X component of the VMagSq vector. Each component of the vector contains the LengthSq
		XMStoreFloat3(&m_direction, v);
	}
	else {
		XMFLOAT3 normalX = XMFLOAT3(1.0f, 0.0f, 0.0f);
		v = XMLoadFloat3(&normalX);
		v = XMVector3Normalize(v);
		XMStoreFloat3(&m_direction, v);
	}
	

}

void Boid::update(float t, vecBoid* boidList, const XMMATRIX& view, const XMMATRIX& proj)
{
	if (!isDead) {
		// create a list of nearby boids
		vecBoid nearBoids = nearbyBoids(boidList);

		// NOTE these functions should always return a normalised vector
		XMFLOAT3  vSeparation = calculateSeparationVector(&nearBoids);
		XMFLOAT3  vAlignment = calculateAlignmentVector(&nearBoids);
		XMFLOAT3  vCohesion = calculateCohesionVector(&nearBoids, boidList);
		XMFLOAT3 vFlee = calculateFleeVector(boidList, view, proj);

		//weight * ( 1 - dist/maxdist)   - power gets bigger the shorter the distance
		//weight * (dist/maxdist)        - power gets smaller the shorter the distance

		//update with strength modifiers
		vSeparation = multiplyFloat3(vSeparation, m_separationWeight);
		vAlignment = multiplyFloat3(vAlignment, m_alignmentWeight);
		vCohesion = multiplyFloat3(vCohesion, m_cohesionWeight);
		vFlee = multiplyFloat3(vFlee, m_fleeWeight);



		XMFLOAT3 vTotal = addFloat3(vCohesion, vAlignment);
		vTotal = addFloat3(vTotal, vSeparation);
		vTotal = addFloat3(vTotal, vFlee);
		float magnitude = magnitudeFloat3(vTotal);
		vTotal = normaliseFloat3(vTotal);

		bool directionChange = false;
		XMFLOAT3 newDirection;
		if (magnitude > 0) {
			newDirection = vTotal;
			directionChange = true;
		}
		else {
			newDirection = XMFLOAT3(0, 0, 0);
		}

		////Rotate the direction vector by X angles. Direction left or right at random.
		//float xRotation;
		//float yRotation;

		//int randomizer = rand() % 3;
		//if (randomizer == 0) {
		//	xRotation = (m_direction.x * cos(DIRECTION_MODIFIER_ANGLE * DELTA_TIME)) - (m_direction.y * sin(DIRECTION_MODIFIER_ANGLE * DELTA_TIME));
		//	yRotation = (m_direction.x * sin(DIRECTION_MODIFIER_ANGLE * DELTA_TIME)) + (m_direction.y * cos(DIRECTION_MODIFIER_ANGLE * DELTA_TIME));
		//}
		//else if (randomizer == 1) {
		//	xRotation = (m_direction.x * cos(DIRECTION_MODIFIER_ANGLE * DELTA_TIME)) - (m_direction.y * sin(DIRECTION_MODIFIER_ANGLE * DELTA_TIME));
		//	yRotation = (m_direction.x * sin(DIRECTION_MODIFIER_ANGLE * DELTA_TIME)) + (m_direction.y * cos(DIRECTION_MODIFIER_ANGLE * DELTA_TIME));
		//}
		//else {
		//	xRotation = m_direction.x;
		//	yRotation = m_direction.y;
		//}
		//m_direction = XMFLOAT3(xRotation, yRotation, 0.0f);
		// 

		if (m_direction.z > 0)
			m_direction.z = 0;
		else if (m_direction.z < 0)
			m_direction.z = 0;

		//damped direction change
		if (directionChange) {
			m_direction = addFloat3(m_direction, multiplyFloat3(newDirection, m_turningDelta));
			normaliseFloat3(m_direction);
		}


		//update the position of the fish, based on direction and speed
		m_position = addFloat3(multiplyFloat3(multiplyFloat3(m_direction, DELTA_TIME), m_speed), m_position);

		// m_direction = ... ;// this should always have a magnitude > 0 - ideally 1
		// m_position = ... ;
	}
	DrawableGameObject::update(t);
}

void Boid::die()
{
	//when boid dies, turn off behaviour and teleport far far away.
	isDead = true;
	m_position = XMFLOAT3(10000, 0, 0);
	
}

XMFLOAT3 Boid::calculateSeparationVector(vecBoid* boidList)
{
	XMFLOAT3 outV = XMFLOAT3(0, 0, 0);
	if (boidList == nullptr)
		return outV;
	int nearbyCount = 0;
	float distanceNearest = -1;
	//find boid that is closest to this boid. steer away from it.
	for (Boid* boid : *boidList) {
		if (boid == this || boid->getFlag() == predatorBoid)
			continue;

		XMFLOAT3 mePos = m_position;
		XMFLOAT3 itPos = *boid->getPosition();
		XMFLOAT3 directionNearest = subtractFloat3(mePos, itPos);
		float d = magnitudeFloat3(directionNearest);
		if (d < m_separationDistance) {
			outV = addFloat3(outV, directionNearest);
			nearbyCount++;
		}

	}


	if (nearbyCount == 0) {
		return XMFLOAT3(0, 0, 0);
	}
	else {
		outV = divideFloat3(outV, nearbyCount);
		//outV = multiplyFloat3(outV, -1);
		return normaliseFloat3(outV);
	}
}

XMFLOAT3 Boid::calculateAlignmentVector(vecBoid* boidList)
{
	XMFLOAT3 outV = XMFLOAT3(0, 0, 0);
	if (boidList == nullptr)
		return outV;

	// your code here
	int nearbyCount = 0;
	for (Boid* boid : *boidList) {
		if (boid == this || boid->getFlag() == predatorBoid)
			continue;
		XMFLOAT3 mePos = m_position;
		XMFLOAT3 itPos = *boid->getPosition();
		XMFLOAT3 directionNearest = subtractFloat3(mePos, itPos);
		float d = magnitudeFloat3(directionNearest);
		if (d < m_sightDistance)
		{
			outV = addFloat3(outV, boid->m_direction);
			nearbyCount++;
		}
	}
	if (nearbyCount == 0) {
		return XMFLOAT3(0, 0, 0);
	}
	else {
		outV = divideFloat3(outV, nearbyCount);
		return normaliseFloat3(outV); // return the normalised (average) direction of nearby drawables
	}
	
}

XMFLOAT3 Boid::calculateCohesionVector(vecBoid* boidList, vecBoid* fullList)
{
	XMFLOAT3 nearby = XMFLOAT3(0, 0, 0);

	if (boidList == nullptr)
		return nearby;

	// calculate average position of nearby
	int nearbyCount = 0;
	float smallFlockNearestDist = -1;
	XMFLOAT3 smallFlockDirectionNearest;

	//if in a small flock, try to get to the nearest boid outside of sight range
	if (boidList->size() < m_smallFlockNumber) {
		m_isInSmallFlock = true;
		for (Boid* boid : *fullList) {
			if (boid == this || boid->getFlag() == predatorBoid) {
				continue;
			}
			XMFLOAT3 mePos = m_position;
			XMFLOAT3 itPos = *boid->getPosition();
			XMFLOAT3 dir = subtractFloat3(itPos, mePos);
			float d = magnitudeFloat3(dir);
			if (d > m_sightDistance)
			{
				if (d < smallFlockNearestDist || smallFlockNearestDist < 0) {
					smallFlockNearestDist = d;
					smallFlockDirectionNearest = dir;
				}
			}
		}
	}
	else {
		m_isInSmallFlock = false;
		for (Boid* boid : *boidList) {
			if (boid == this || boid->getFlag() == predatorBoid) {
				continue;
			}
			XMFLOAT3 mePos = m_position;
			XMFLOAT3 itPos = *boid->getPosition();
			XMFLOAT3 dir = subtractFloat3(mePos, itPos);
			float d = magnitudeFloat3(dir);
			if (d < m_sightDistance && d > m_separationDistance)
			{
				nearby = addFloat3(nearby, itPos);
				nearbyCount++;
			}
		}
	}
	nearby = divideFloat3(nearby, nearbyCount);
	nearby = subtractFloat3(nearby, m_position); 

	//depending on how far the average position is, weight the cohesion down
	float d = magnitudeFloat3(nearby);
	nearby = normaliseFloat3(nearby); // nearby (when normalised) is the direction to where the other drawables are
	if (m_isInSmallFlock) {
		return multiplyFloat3(normaliseFloat3(smallFlockDirectionNearest), m_cohesionMultiplier);
	}
	else if (d < COHESION_DISTANCE) { //but only if below cohesion distance
		float weight = (d / COHESION_DISTANCE);
		return multiplyFloat3(nearby, weight);
	}
	else {
		return multiplyFloat3(nearby, m_cohesionMultiplier);; //if distance is far then multiply!
	}
	 

	
}

XMFLOAT3 Boid::calculateFleeVector(vecBoid* fullList, const XMMATRIX& view, const XMMATRIX& proj)
{
	XMFLOAT3 outV = XMFLOAT3(0, 0, 0);
	int predatorCount = 0;
	if (fullList == nullptr)
		return outV;

	//find predators that are close to this boid.
	for (int i = 0; i < PREDATOR_COUNT; i++) {

		XMFLOAT3 mePos = m_position;
		XMFLOAT3 itPos = *fullList->at(i)->getPosition();
		XMFLOAT3 direction = subtractFloat3(mePos, itPos);
		float d = magnitudeFloat3(direction);
		if (d < m_sightDistance) {
			XMFLOAT3 future = addFloat3(multiplyFloat3(multiplyFloat3(direction, DELTA_TIME), m_speed), m_position);

			XMFLOAT4 v4;
			v4.x = future.x;
			v4.y = future.y;
			v4.z = future.z;
			v4.w = 1.0f;

			XMVECTOR vScreenSpace = XMLoadFloat4(&v4);
			XMVECTOR vScreenSpace2 = XMVector4Transform(vScreenSpace, view);
			XMVECTOR vScreenSpace3 = XMVector4Transform(vScreenSpace2, proj);

			XMFLOAT4 v;
			XMStoreFloat4(&v, vScreenSpace3);
			v.x /= v.w;
			v.y /= v.w;
			v.z /= v.w;
			v.w /= v.w;
			float fOffset = 10; // a suitable distance to rectify position within clip space
			if (v.x < -1 || v.x > 1 || v.y < -1 || v.y > 1)
			{
				if (v.x < -1 || v.x > 1) {
					v4.x = -v4.x + (fOffset * v.x);
				}
				else if (v.y < -1 || v.y > 1) {
					v4.y = -v4.y + (fOffset * v.y);
				}

				if (v.y < -1 || v.y > 1)
				{
					direction.y *= -1.0f;
				}
				if (v.x < -1 || v.x > 1)
				{
					direction.y *= -1.0f;
				}
			}
			direction = normaliseFloat3(direction);
			direction.z = 0;
			outV = addFloat3(outV, direction);
			predatorCount++;
		}
	}
	if (predatorCount == 0) {
		return XMFLOAT3(0, 0, 0);
	}
	else {
		outV = normaliseFloat3(outV);
		return outV;
	}
}

Boid* Boid::findClosestBoid(vecBoid* boidList)
{
	if (boidList == nullptr)
		return nullptr;
	Boid* outBoid = nullptr;
	int nearbyCount = 0;
	float distanceNearest = -1;
	//find boid that is closest to this boid
	for (Boid* boid : *boidList) {
		if (boid == this || boid->getFlag() == predatorBoid)
			continue;

		XMFLOAT3 mePos = m_position;
		XMFLOAT3 itPos = *boid->getPosition();
		XMFLOAT3 directionNearest = subtractFloat3(itPos, mePos);
		float d = magnitudeFloat3(directionNearest);
		if ((d < distanceNearest) || distanceNearest < 0) {
			distanceNearest = d;
			outBoid = boid;
		}
		nearbyCount++;

	}


	if (outBoid == nullptr) {
		return nullptr;
	}
	else {
		return outBoid;
	}
}



// use but don't alter the methods below

XMFLOAT3 Boid::addFloat3(const XMFLOAT3& f1, const XMFLOAT3& f2)
{
	XMFLOAT3 out;
	out.x = f1.x + f2.x;
	out.y = f1.y + f2.y;
	out.z = f1.z + f2.z;

	return out;
}

XMFLOAT3 Boid::subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2)
{
	XMFLOAT3 out;
	out.x = f1.x - f2.x;
	out.y = f1.y - f2.y;
	out.z = f1.z - f2.z;

	return out;
}

XMFLOAT3 Boid::multiplyFloat3(const XMFLOAT3& f1, const float scalar)
{
	XMFLOAT3 out;
	out.x = f1.x * scalar;
	out.y = f1.y * scalar;
	out.z = f1.z * scalar;

	return out;
}

XMFLOAT3 Boid::divideFloat3(XMFLOAT3& f1, const float scalar)
{
	XMFLOAT3 out;
	out.x = f1.x / scalar;
	out.y = f1.y / scalar;
	out.z = f1.z / scalar;

	return out;
}

bool Boid::checkCollision(Boid* other)
{
	//optimize by only doing the dereference once. requires a small amount of extra memory.
	float meBounds = (m_scale / 2) * COLLISION_BOUNDS_MULTIPLIER; 
	float itBounds = (other->getScale() / 2) * COLLISION_BOUNDS_MULTIPLIER;
	XMFLOAT3 itPos = *other->getPosition();


	//check square collision between this boid and another boid. return true if collision occured. 
	if (m_position.x + meBounds > itPos.x - itBounds &&
		m_position.x - meBounds < itPos.x + itBounds &&
		m_position.y + meBounds > itPos.y - itBounds &&
		m_position.y - meBounds < itPos.y + itBounds) {
		return true;
	}
	else return false;
}

bool Boid::checkCollisionCircle(Boid* other)
{
	float meRadius = (m_scale / 2) * COLLISION_BOUNDS_MULTIPLIER;
	float itRadius = (other->getScale() / 2) * COLLISION_BOUNDS_MULTIPLIER;
	XMFLOAT3 itPos = *other->getPosition();
	XMFLOAT3 difference = subtractFloat3(itPos, m_position);
	if (magnitudeFloat3(difference) < meRadius + itRadius)
		return true;
	else return false;
}

string Boid::returnDiagString()
{
	void* mypntr = this;
	std::stringstream outss;
	outss << mypntr << " died at " << m_timeOfDeath << ". Parameters:  " << std::endl <<
														"Sight range : " << m_sightDistance << std::endl <<
														"Sight degrees : " << m_sightArc << std::endl <<
														"Turning ability : " << m_turningDelta << std::endl << 
														"Cohesion Weight : " << m_cohesionWeight << std::endl <<
														"Cohesion Multiplier : " << m_cohesionMultiplier << std::endl <<
														"Separation Weight : " << m_separationWeight << std::endl <<
														"Alignment Weight : " << m_alignmentWeight << std::endl <<
														"Flee Weight : " << m_fleeWeight << std::endl <<
														"Small Flock definition : " << m_smallFlockNumber << std::endl <<
														"There was " << m_boidsAtDeath << " nearby boids at time of death" << ". The boid was located at : x: " << m_deathPosition.x << " y : " << m_deathPosition.y << endl << endl;

	return outss.str();
}

void Boid::findDeathData(vecBoid* fullList, float time)
{
	m_boidsAtDeath = nearbyBoids(fullList).size();
	m_deathPosition = m_position;
	m_timeOfDeath = time;
}

float Boid::magnitudeFloat3(XMFLOAT3& f1)
{
	return sqrt((f1.x * f1.x) + (f1.y * f1.y) + (f1.z * f1.z));
}

float Boid::dotFloat3(const XMFLOAT3& f1, const XMFLOAT3& f2)
{
	return f1.x * f2.x + f1.y * f2.y + f1.z * f2.z;
}

XMFLOAT3 Boid::normaliseFloat3(XMFLOAT3& f1)
{
	float length = sqrt((f1.x * f1.x) + (f1.y * f1.y) + (f1.z * f1.z));

	f1.x /= length;
	f1.y /= length;
	f1.z /= length;

	return f1;
}

vecBoid Boid::nearbyBoids(vecBoid* boidList)
{
	vecBoid nearBoids;
	if (boidList->size() == 0)
		return nearBoids;

	for (Boid* boid : *boidList) {
		// ignore self
		if (boid == this || boid->getFlag() == predatorBoid)
			continue;

		// get the distance between the two
		XMFLOAT3 vB = *(boid->getPosition());
		XMFLOAT3 vDiff = subtractFloat3(m_position, vB);
		float l = magnitudeFloat3(vDiff);
		if (l < m_sightDistance) {
			XMFLOAT3 normalisedDirection = m_direction;
			float dot = dotFloat3(normaliseFloat3(vDiff), normaliseFloat3(normalisedDirection));
			if (dot < -1)
				dot = -1;
			else if (dot > 1)
				dot = 1;
			float angle = acos(dot);
			angle = angle * (180.0f / 3.14159265358979323846f);
			if(angle < m_sightArc / 2)
				nearBoids.push_back(boid);
		}
	}

	return nearBoids;
}


void Boid::checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj)
{

	XMFLOAT4 v4;
	v4.x = m_position.x;
	v4.y = m_position.y;
	v4.z = m_position.z;
	v4.w = 1.0f;

	XMVECTOR vScreenSpace = XMLoadFloat4(&v4);
	XMVECTOR vScreenSpace2 = XMVector4Transform(vScreenSpace, view);
	XMVECTOR vScreenSpace3 = XMVector4Transform(vScreenSpace2, proj);

	XMFLOAT4 v;
	XMStoreFloat4(&v, vScreenSpace3);
	v.x /= v.w;
	v.y /= v.w;
	v.z /= v.w;
	v.w /= v.w;

	float fOffset = 10; // a suitable distance to rectify position within clip space
	if (v.x < -1 || v.x > 1 || v.y < -1 || v.y > 1)
	{
		if (v.x < -1 || v.x > 1) {
			v4.x = -v4.x + (fOffset * v.x);
		}
		else if (v.y < -1 || v.y > 1) {
			v4.y = -v4.y + (fOffset * v.y);
		}

		// throw a bit of randomness into the mix
		//createRandomDirection();

		// method 1 - appear on the other side
		if (WRAP_METHOD > 0) {
			m_position.x = v4.x;
			m_position.y = v4.y;
			m_position.z = v4.z;
		}
		// method2 - bounce off sides and head to centre
		else {
			if ((v.y < -1 || v.y > 1) && (v.x < -1 || v.x > 1))
			{
				if (v.y < -1 && v.x < -1)
				{
					if (m_direction.x < 0)
					{
						m_direction.x *= -1.0;
					}
					else if (m_direction.y < 0)
					{
						m_direction.y *= -1.0;
					}
				}
				else if (v.y < -1 && v.x > 1)
				{
					if (m_direction.x > 0)
					{
						m_direction.x *= -1.0;
					}
					else if (m_direction.y < 0)
					{
						m_direction.y *= -1.0;
					}
				}
				else if (v.y > 1 && v.x < -1)
				{
					if (m_direction.x < 0)
					{
						m_direction.x *= -1.0;
					}
					else if (m_direction.y > 0)
					{
						m_direction.y *= -1.0;
					}
				}
				else if (v.y > 1 && v.x > 1)
				{
					if (m_direction.x > 0)
					{
						m_direction.x *= -1.0;
					}
					else if (m_direction.y > 0)
					{
						m_direction.y *= -1.0;
					}
				}

				m_direction = normaliseFloat3(m_direction);
				setDirection(m_direction);
			}

			else if (v.x < -1 || v.x > 1)
			{
				m_direction.x *= -1.0;
				m_direction = normaliseFloat3(m_direction);
				setDirection(m_direction);
			}
			else if (v.y < -1 || v.y > 1)
			{
				m_direction.y *= -1.0;
				m_direction = normaliseFloat3(m_direction);
				setDirection(m_direction);
			}
		}
	}

	return;
}