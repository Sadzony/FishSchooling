#pragma once

#define DELTA_TIME 0.018f

#define FISH_COUNT 400
#define FISH_SPEED 25.0f
#define NEARBY_DISTANCE		25.0f	// how far boids can see


#define SEPARATION_DISTANCE 6.0f //distance on which separation is multiplied
#define SEPARATION_MULTIPLIER 4.0f // multiplier for strength when separation distance is reached

#define COHESION_DISTANCE 14.0f
#define COHESION_MULTIPLIER 2.5f
#define SMALL_FLOCK_NUMBER 10

#define ALIGNMENT_STRENGTH 3.0f
#define SEPARATION_STRENGTH 1.5f
#define COHESION_STRENGTH 1.8f

#define SMOOTH_VALUE_FISH 0.06f
#define SMOOTH_VALUE_PREDATOR 0.03f