#pragma once

#define DELTA_TIME 0.018f

#define FISH_COUNT 250
#define PREDATOR_COUNT 6

#define FISH_SCALE 1.375
#define PREDATOR_SCALE 6

#define FISH_SPEED 23.0f
#define PREDATOR_SPEED 34.0f

#define BOID_SIGHT_RANGE		70.0f	// how far boids can see
#define BOID_SIGHT_VARIANCE 0.6f //what fraction of sight range does the randomizer vary in
#define PREDATOR_SIGHT_RANGE 1000.0f
#define PREDATOR_FOCUS_RANGE 45.0f


#define SEPARATION_DISTANCE 6.0f //distance on which separation is multiplied
#define SEPARATION_MULTIPLIER 5.0f // multiplier for strength when separation distance is reached

#define COHESION_DISTANCE 14.0f
#define COHESION_MULTIPLIER 3.0f
#define SMALL_FLOCK_NUMBER 12

#define ALIGNMENT_STRENGTH 5.0f
#define SEPARATION_STRENGTH 2.9f
#define COHESION_STRENGTH 3.2f

#define FLEE_STRENGTH 30.0f


#define SMOOTH_VALUE_FISH 0.033f
#define SMOOTH_VALUE_VARIANCE 0.7f //what fraction of smooth delta does the randomizer vary in
#define SMOOTH_VALUE_PREDATOR 0.022f

#define COLLISION_BOUNDS_MULTIPLIER 2.5f //makes the bounds x times bigger. using this to correct for fast movement.

#define WRAP_METHOD -1 //-1 for bouncing off sides. 1 for coming out the other side

#define DIAGNOSTIC_TIME 25000 //time in milliseconds before printing diagnostic file

#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 1080
