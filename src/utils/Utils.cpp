#include "Utils.h"

#include <cstdlib>

float Utils::Random(float min, float max)
{
    return (float)(min + (max - min) * ((double)rand() / (double)RAND_MAX));
}

float Utils::RandomVariation(float center, float variation)
{
    return Random(center - variation, center + variation);
}