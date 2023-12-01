#pragma once
#include <cstdlib>
#include <algorithm>

void seedRandom(unsigned int seed)
{
	std::srand(seed);
}

float random01()
{
	return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

float random(float min, float max)
{
    if (min > max) {
        std::swap(min, max); 
    }

    return min + (random01() * (max - min));
}
