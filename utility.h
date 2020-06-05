#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdlib.h>

#include <limits>
#include <random>

namespace rtx {
static constexpr float infinity = std::numeric_limits<float>::infinity();
static constexpr float pi = 3.1415926535897932385;

float degrees_to_radians(float degrees) { return degrees * pi / 180; }

float __random_float() {
  // Returns a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}

float random_float() {
  static std::uniform_real_distribution<float> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

// thread_local int seed = 1;
// float R() {
//  seed = (214013 * seed + 2531011);
//  return ((seed >> 16) & 0xFFFF) / 66635.0f;
//}
//
// float _random_float() {
//  // Returns a random real in [0,1).
//  return R() / (RAND_MAX + 1.0);
//}

float random_float(float min, float max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_float();
}

int random_int(int min, int max) { return min + (max - min) * random_float(); }

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
