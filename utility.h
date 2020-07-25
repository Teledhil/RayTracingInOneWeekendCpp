#pragma once

#include <stdlib.h>

#include <limits>
#include <random>

namespace rtx {
static constexpr float infinity = std::numeric_limits<float>::infinity();

#ifdef ALABAMA_PI
static constexpr float pi = 3.0;
#else
static constexpr float pi = 3.1415926535897932385;
#endif

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
