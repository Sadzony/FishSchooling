#pragma once
#include "DrawableGameObject.h"
#include "defines.h"
enum FishFlag {
	fishBoid,
	predatorBoid
};
class Boid :
	public DrawableGameObject
{
public:
	Boid();
	~Boid();

	XMFLOAT3*							getDirection() { return &m_direction; }
	FishFlag							getFlag() { return m_flag; }
	float								getScale() { return m_scale; }
	void								checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj);
	virtual void						update(float t, vecBoid* drawList);

	bool								m_isDead;
	void								die();
	bool								checkCollision(Boid* other);

protected:
	void								setDirection(XMFLOAT3 direction);

	vecBoid								nearbyBoids(vecBoid* boidList);
	XMFLOAT3							calculateSeparationVector(vecBoid* drawList);
	XMFLOAT3							calculateAlignmentVector(vecBoid* drawList);
	XMFLOAT3							calculateCohesionVector(vecBoid* drawList, vecBoid* fullList);
	Boid*								findClosestBoid(vecBoid* boidList);
	void								createRandomDirection();
	void								randomizeStats();
	float								randomizeWithinFraction(double value, float fraction);

	XMFLOAT3							addFloat3(const XMFLOAT3& f1, const XMFLOAT3& f2);
	XMFLOAT3							subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							normaliseFloat3(XMFLOAT3& f1);
	float								magnitudeFloat3(XMFLOAT3& f1);
	XMFLOAT3							multiplyFloat3(const XMFLOAT3& f1, const float scalar);
	XMFLOAT3							divideFloat3(XMFLOAT3& f1, const float scalar);





	XMFLOAT3							m_direction;
	float								m_speed;
	float								m_sightDistance;
	float								m_scale;
	float								m_turningDelta;
	FishFlag							m_flag;
	//unsigned int*						m_nearbyDrawables;
};

