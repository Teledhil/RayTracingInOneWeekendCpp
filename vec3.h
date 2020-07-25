#ifndef VEC3_H_
#define VEC3_H_

#include <math.h>

#include <iostream>

#include "utility.h"

namespace rtx {
class vec3 {
  public:
    vec3() : v{0, 0, 0} {}
    vec3(float x, float y, float z) : v{x, y, z} {}

    float x() const { return v[0]; }
    float y() const { return v[1]; }
    float z() const { return v[2]; }

    vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); }
    float operator[](int i) const { return v[i]; }
    float &operator[](int i) { return v[i]; }

    vec3 &operator+=(const vec3 &o) {
      v[0] += o.v[0];
      v[1] += o.v[1];
      v[2] += o.v[2];
      return *this;
    }

    vec3 &operator*=(float t) {
      v[0] *= t;
      v[1] *= t;
      v[2] *= t;
      return *this;
    }

    vec3 &operator/=(float t) { return *this *= 1 / t; }

    float length_squared() const {
      return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    }
    float length() const { return std::sqrt(length_squared()); }

    bool is_bad() const {
      // To check if some value is NaN.
      if (v[0] != v[0]) {
        return true;
      }
      if (v[1] != v[1]) {
        return true;
      }
      if (v[2] != v[2]) {
        return true;
      }
      return false;
    }

    bool operator==(const vec3 &o) const {
      if (v[0] != o.v[0]) {
        return false;
      }
      if (v[1] != o.v[1]) {
        return false;
      }
      if (v[2] != o.v[2]) {
        return false;
      }

      return true;
    }

    float min() const { return std::min(v[0], std::min(v[1], v[2])); }
    float max() const { return std::max(v[0], std::max(v[1], v[2])); }

  private:
    float v[3];
};

using point3 = vec3;
using color = vec3; // RGB

// utility functions

std::ostream &operator<<(std::ostream &stream, const vec3 &v) {
  stream << v.x() << " " << v.y() << " " << v.z();
  return stream;
}

vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

vec3 operator*(const vec3 &v, float t) {
  return vec3(v.x() * t, v.y() * t, v.z() * t);
}
vec3 operator*(float t, const vec3 &v) { return v * t; }

vec3 operator/(vec3 v, float t) { return (1 / t) * v; }
vec3 operator/(float t, vec3 v) {
  return vec3(t / v.x(), t / v.y(), t / v.z());
}
vec3 inv(vec3 v) { return vec3(1.0 / v.x(), 1.0 / v.y(), 1.0 / v.z()); }

float dot(const vec3 &u, const vec3 &v) {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
              u.x() * v.y() - u.y() * v.x());
}

float min(const vec3 &v) { return v.min(); }
float max(const vec3 &v) { return v.max(); }

vec3 min(const vec3 &u, const vec3 &v) {
  return vec3(std::min(u.x(), v.x()), std::min(u.y(), v.y()),
              std::min(u.z(), v.z()));
}
vec3 max(const vec3 &u, const vec3 &v) {
  return vec3(std::max(u.x(), v.x()), std::max(u.y(), v.y()),
              std::max(u.z(), v.z()));
}

vec3 unit_vector(vec3 v) {
  if (0 == v.length()) {
    std::cerr << "zero division" << std::endl;
  }
  return v / v.length();
}


vec3 reflect(const vec3 &v, const vec3 &n) { return v - 2 * dot(v, n) * n; }
vec3 refract(const vec3 &r_in, const vec3 &n, float etai_over_etat) {
  // Snell's Law.
  //
  // R′ = R′∥ + R′⊥
  // R′∥ = η / η′ * (R + cosθ * n)
  // R′⊥ = −√(1 - |R′∥|²) * n
  float cos_theta = dot(-r_in, n);
  vec3 r_out_parallel = etai_over_etat * (r_in + cos_theta * n);
  vec3 r_out_perpendicular = -sqrt(1.0 - r_out_parallel.length_squared()) * n;

  return r_out_parallel + r_out_perpendicular;
}

} // namespace rtx
# endif // VEC3_H_
