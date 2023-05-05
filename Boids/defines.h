#pragma once

#define DELTA_TIME 0.018f

#define FISH_COUNT 400
#define PREDATOR_COUNT 1

#define FISH_SCALE 1
#define PREDATOR_SCALE 7

#define FISH_SPEED 35.0f
#define FISH_SPEED_VARIANCE 0.75f //what fraction of speed does the randomizer vary in
#define PREDATOR_SPEED 45.0f

#define BOID_SIGHT_RANGE		40.0f	// how far boids can see
#define BOID_SIGHT_VARIANCE 0.6f //what fraction of sight range does the randomizer vary in
#define PREDATOR_SIGHT_RANGE 300.0f
#define PREDATOR_FOCUS_RANGE 9.0f


#define SEPARATION_DISTANCE 6.0f //distance on which separation is multiplied
#define SEPARATION_MULTIPLIER 4.0f // multiplier for strength when separation distance is reached

#define COHESION_DISTANCE 14.0f
#define COHESION_MULTIPLIER 2.5f
#define SMALL_FLOCK_NUMBER 8

#define ALIGNMENT_STRENGTH 3.0f
#define SEPARATION_STRENGTH 1.5f
#define COHESION_STRENGTH 1.8f

#define FLEE_STRENGTH 30.0f


#define SMOOTH_VALUE_FISH 0.065f
#define SMOOTH_VALUE_VARIANCE 0.7f //what fraction of smooth delta does the randomizer vary in
#define SMOOTH_VALUE_PREDATOR 0.025f

#define COLLISION_BOUNDS_MULTIPLIER 2.5f //makes the bounds x times bigger. using this to correct for fast movement.

#define WRAP_METHOD 1 //-1 for bouncing off sides. 1 for coming out the other side

#define DIAGNOSTIC_TIME 60000 //time in milliseconds before printing diagnostic file