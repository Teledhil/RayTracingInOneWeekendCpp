#ifndef UTILITY_H_
#define UTILITY_H_

#include <limits>

static constexpr double infinity = std::numeric_limits<double>::infinity();
static constexpr double pi = 3.1415926535897932385;

double degrees_to_radians(double degrees);

double random_double();
double random_double(double min, double max);

double clamp(double x, double min, double max);

#endif // UTILITY_H_
