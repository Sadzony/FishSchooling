#pragma once
#include "DrawableGameObject.h"
#include "defines.h"
#include "sstream"
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
	bool								getSmallFlock() { return m_isInSmallFlock; }
	void								checkIsOnScreenAndFix(const XMMATRIX&  view, const XMMATRIX&  proj);
	virtual void						update(float t, vecBoid* drawList, const XMMATRIX& view, const XMMATRIX& proj);

	bool								isDead = false;
	void								die();
	bool								checkCollision(Boid* other);
	bool								checkCollisionCircle(Boid* other);

	string								returnDiagString();
	void								findDeathData(vecBoid* fullList, float time);

protected:
	void								setDirection(XMFLOAT3 direction);

	vecBoid								nearbyBoids(vecBoid* boidList);

	XMFLOAT3							calculateSeparationVector(vecBoid* drawList);
	XMFLOAT3							calculateAlignmentVector(vecBoid* drawList);
	XMFLOAT3							calculateCohesionVector(vecBoid* drawList, vecBoid* fullList);
	XMFLOAT3							calculateFleeVector(vecBoid* fullList, const XMMATRIX& view, const XMMATRIX& proj);
								

	virtual Boid*						findClosestBoid(vecBoid* boidList);
	void								createRandomDirection();
	void								randomizeStats();
	float								randomizeWithinFraction(double value, float fraction);

	XMFLOAT3							addFloat3(const XMFLOAT3& f1, const XMFLOAT3& f2);
	XMFLOAT3							subtractFloat3(XMFLOAT3& f1, XMFLOAT3& f2);
	XMFLOAT3							normaliseFloat3(XMFLOAT3& f1);
	float								magnitudeFloat3(XMFLOAT3& f1);
	float								dotFloat3(const XMFLOAT3& f1, const XMFLOAT3& f2);
	XMFLOAT3							multiplyFloat3(const XMFLOAT3& f1, const float scalar);
	XMFLOAT3							divideFloat3(XMFLOAT3& f1, const float scalar);





	XMFLOAT3							m_direction;
	float								m_speed;
	float								m_sightDistance;
	float								m_sightArc;
	float								m_scale;
	float								m_turningDelta;
	float								m_cohesionWeight;
	float								m_cohesionMultiplier;
	float								m_separationWeight;
	float								m_separationDistance;
	float								m_alignmentWeight;
	float								m_fleeWeight;
	int									m_smallFlockNumber;
	FishFlag							m_flag;
	bool								m_isInSmallFlock = false;
	float								m_timeOfDeath = 0.0f;
	int									m_boidsAtDeath = 0;
	XMFLOAT3							m_deathPosition;
	//unsigned int*						m_nearbyDrawables;
};

