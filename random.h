#pragma once

#include <random>

#include "vec3.h"

namespace rtx {

class random {
public:
  random(int seed) : distribution_(0.0, 1.0), generator_(seed) {}
  random() : distribution_(0.0, 1.0), generator_(5489u) {}

  float random_float() { return distribution_(generator_); }

  float random_float(float min, float max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_float();
  }

  int random_int(int min, int max) {
    return min + (max - min) * random_float();
  }

  vec3 random_vec3() {
    return vec3(random_float(), random_float(), random_float());
  }
  vec3 random_vec3(float min, float max) {
    return vec3(random_float(min, max), random_float(min, max),
                random_float(min, max));
  }

  vec3 random_in_unit_sphere() {
    while (true) {
      vec3 p = random_vec3(-1, 1);
      if (p.length_squared() >= 1) {
        continue;
      }
      return p;
    }
  }
  vec3 random_in_unit_disk() {
    while (true) {
      vec3 p = vec3(random_float(-1, 1), random_float(-1, 1), 0);
      if (p.length_squared() >= 1) {
        continue;
      }
      return p;
    }
  }
  vec3 random_unit_vector() {
    // Lambertian's cosine law.
    float angle = random_float(0, 2 * pi);
    float z = random_float(-1, 1);
    float radiance = sqrt(1 - z * z);
    return vec3(radiance * cos(angle), radiance * sin(angle), z);
  }
  vec3 random_in_hemisphere(const vec3 &normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) {
      // In the same hemisphere as the normal
      return in_unit_sphere;
    } else {
      return -in_unit_sphere;
    }
  }

  private:
    std::uniform_real_distribution<float> distribution_;
    std::mt19937 generator_;
  };
}
