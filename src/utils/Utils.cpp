#include "Utils.h"

#include <cstdlib>

float Utils::random(float min, float max)
{
    return (float)(min + (max - min) * ((double)rand() / (double)RAND_MAX));
}

float Utils::randomVariation(float center, float variation)
{
    return random(center - variation, center + variation);
}
