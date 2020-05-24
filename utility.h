#ifndef UTILITY_H_
#define UTILITY_H_

#include <limits>

static constexpr float infinity = std::numeric_limits<float>::infinity();
static constexpr float pi = 3.1415926535897932385;

float degrees_to_radians(float degrees);

float random_float();
float random_float(float min, float max);

float clamp(float x, float min, float max);

#endif // UTILITY_H_
