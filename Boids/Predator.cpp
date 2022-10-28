#include "Predator.h"

Predator::Predator() 
{
	m_flag = predatorBoid;
	m_position = XMFLOAT3(0, 0, 0);
	m_direction = XMFLOAT3(0, 1, 0);
	createRandomDirection();
	setScale(5);
}

Predator::~Predator()
{
}

void Predator::update(float t, vecBoid* drawList)
{
	DrawableGameObject::update(t);
}
