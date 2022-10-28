#pragma once
#include "Boid.h"
class Predator :
    public Boid
{
public:
    Predator();
    ~Predator();
    virtual void update(float t, vecBoid* drawList) override;
};

