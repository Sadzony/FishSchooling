#pragma once
#include "Boid.h"
class Predator :
    public Boid
{
public:
    Predator();
    ~Predator();
    virtual void update(float t, vecBoid* drawList) override;
    bool checkCollisionPredator();
    virtual Boid* findClosestBoid(vecBoid* boidList) override;

    Boid* getNearestBoid() { return m_nearestBoid; }
protected:
    Boid* m_nearestBoid;
    float m_distanceToTarget;
};

