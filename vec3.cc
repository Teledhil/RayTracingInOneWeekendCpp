#include "vec3.h"

#include "utility.h"

#include <iostream>

vec3 vec3::random() {
  return vec3(random_double(), random_double(), random_double());
}
vec3 vec3::random(double min, double max) {
  return vec3(random_double(min, max), random_double(min, max),
              random_double(min, max));
}

vec3 vec3::random_in_unit_sphere() {
  while (true) {
    vec3 p = vec3::random(-1, 1);
    if (p.length_squared() >= 1) {
      continue;
    }
    return p;
  }
}

vec3 vec3::random_in_unit_disk() {
  while (true) {
    vec3 p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
    if (p.length_squared() >= 1) {
      continue;
    }
    return p;
  }
}

vec3 vec3::random_unit_vector() {
  // Lambertian's cosine law.
  double angle = random_double(0, 2 * pi);
  double z = random_double(-1, 1);
  double radiance = sqrt(1 - z * z);
  return vec3(radiance * cos(angle), radiance * sin(angle), z);
}

vec3 vec3::random_in_hemisphere(const vec3 &normal) {
  vec3 in_unit_sphere = random_in_unit_sphere();
  if (dot(in_unit_sphere, normal) > 0.0) {
    // In the same hemisphere as the normal
    return in_unit_sphere;
  } else {
    return -in_unit_sphere;
  }
}

vec3 vec3::reflect(const vec3 &v, const vec3 &n) {
  return v - 2 * dot(v, n) * n;
}

vec3 vec3::refract(const vec3 &r_in, const vec3 &n, double etai_over_etat) {
  // Snell's Law.
  //
  // R′ = R′∥ + R′⊥
  // R′∥ = η / η′ * (R + cosθ * n)
  // R′⊥ = −√(1 - |R′∥|²) * n
  double cos_theta = dot(-r_in, n);
  vec3 r_out_parallel = etai_over_etat * (r_in + cos_theta * n);
  vec3 r_out_perpendicular = -sqrt(1.0 - r_out_parallel.length_squared()) * n;

  return r_out_parallel + r_out_perpendicular;
}

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

vec3 operator*(const vec3 &v, double t) {
  return vec3(v.x() * t, v.y() * t, v.z() * t);
}
vec3 operator*(double t, const vec3 &v) { return v * t; }

vec3 operator/(vec3 v, double t) { return (1 / t) * v; }

double dot(const vec3 &u, const vec3 &v) {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
              u.x() * v.y() - u.y() * v.x());
}

vec3 unit_vector(vec3 v) { return v / v.length(); }
