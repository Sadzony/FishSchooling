#include "Predator.h"

Predator::Predator() 
{
	m_flag = predatorBoid;
	m_position = XMFLOAT3(0, 0, 0);
	m_direction = XMFLOAT3(0, 1, 0);
	m_sightDistance = PREDATOR_SIGHT_RANGE;
	m_turningDelta = SMOOTH_VALUE_PREDATOR;
	m_speed = PREDATOR_SPEED;
	createRandomDirection();
	setScale(5);
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
	Boid* closest = findClosestBoid(&nearBoids);

	if (closest != nullptr) {
		directionChange = true;
		XMFLOAT3 mePos = m_position;
		XMFLOAT3 itPos = *closest->getPosition();
		newDirection = subtractFloat3(itPos, mePos);
		normaliseFloat3(newDirection);
	}

	if (directionChange) {
		m_direction = addFloat3(m_direction, multiplyFloat3(newDirection, m_turningDelta));
		normaliseFloat3(m_direction);
	}

	m_position = addFloat3(multiplyFloat3(multiplyFloat3(m_direction, DELTA_TIME), m_speed), m_position);
	DrawableGameObject::update(t);
}
