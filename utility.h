#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdlib.h>

#include <limits>
#include <random>

namespace rtx {
static constexpr float infinity = std::numeric_limits<float>::infinity();
static constexpr float pi = 3.1415926535897932385;

float degrees_to_radians(float degrees) { return degrees * pi / 180; }

float clamp(float x, float min, float max) {
  if (x < min) {
    return min;
  }
  if (x > max) {
    return max;
  }
  return x;
}

} // namespace rtx
#endif // UTILITY_H_
