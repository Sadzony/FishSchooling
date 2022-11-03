#include "Predator.h"

Predator::Predator() 
{
	m_distanceToTarget = 0;
	m_nearestBoid = nullptr;
	m_flag = predatorBoid;
	m_position = XMFLOAT3(0, 0, 0);
	m_direction = XMFLOAT3(0, 1, 0);
	m_sightDistance = PREDATOR_SIGHT_RANGE;
	m_turningDelta = SMOOTH_VALUE_PREDATOR;
	m_speed = PREDATOR_SPEED;
	createRandomDirection();
	setScale(PREDATOR_SCALE);
}

Predator::~Predator()
{
}

void Predator::update(float t, vecBoid* boidList)
{
	bool directionChange = false;
	XMFLOAT3 newDirection = XMFLOAT3(0, 0, 0);

	// create a list of nearby boids
	vecBoid nearBoids = nearbyBoids(boidList);
	m_nearestBoid = findClosestBoid(&nearBoids);

	if (m_nearestBoid != nullptr) {
		directionChange = true;
		XMFLOAT3 mePos = m_position;
		XMFLOAT3 itPos = *m_nearestBoid->getPosition();
		newDirection = subtractFloat3(itPos, mePos);
		m_distanceToTarget = magnitudeFloat3(newDirection);
		normaliseFloat3(newDirection);
	}

	if (directionChange) {
		m_direction = addFloat3(m_direction, multiplyFloat3(newDirection, m_turningDelta));
		normaliseFloat3(m_direction);
	}

	m_position = addFloat3(multiplyFloat3(multiplyFloat3(m_direction, DELTA_TIME), m_speed), m_position);
	DrawableGameObject::update(t);
}

bool Predator::checkCollisionPredator()
{
	if (m_nearestBoid != nullptr) {
		float meRadius = (PREDATOR_SCALE / 2) * COLLISION_BOUNDS_MULTIPLIER;
		float itRadius = (FISH_SCALE / 2) * COLLISION_BOUNDS_MULTIPLIER;
		if (m_distanceToTarget < meRadius + itRadius)
			return true;
		else return false;
	}
	return false;
}

Boid* Predator::findClosestBoid(vecBoid* boidList)
{
	if (boidList == nullptr)
		return nullptr;
	Boid* outBoid = nullptr;
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
			//ignore boids in small flocks, but not if theyre close
			if (boid->getSmallFlock() == false || d < PREDATOR_FOCUS_RANGE) {
				distanceNearest = d;
				outBoid = boid;
			}
		}

	}


	if (outBoid == nullptr) {
		return nullptr;
	}
	else {
		return outBoid;
	}
}
