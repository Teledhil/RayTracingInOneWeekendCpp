#ifndef VEC3_H_
#define VEC3_H_

#include <math.h>

#include <iostream>

class vec3 {
  public:
    vec3() : v{0, 0, 0} {}
    vec3(float x, float y, float z) : v{x, y, z} {}
    vec3(const vec3 &other) {
      v[0] = other.v[0];
      v[1] = other.v[1];
      v[2] = other.v[2];
    }

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

    static vec3 random();
    static vec3 random(float min, float max);

    static vec3 random_in_unit_sphere();
    static vec3 random_in_unit_disk();
    static vec3 random_unit_vector();
    static vec3 random_in_hemisphere(const vec3 &normal);

    static vec3 reflect(const vec3 &v, const vec3 &n);
    static vec3 refract(const vec3 &r_in, const vec3 &n, float etai_over_etat);

  private:
    float v[3];
};

using point3 = vec3;
using color = vec3; // RGB

// utility functions

std::ostream &operator<<(std::ostream &stream, const vec3 &v);

vec3 operator+(const vec3 &u, const vec3 &v);

vec3 operator-(const vec3 &u, const vec3 &v);

vec3 operator*(const vec3 &u, const vec3 &v);

vec3 operator*(const vec3 &v, float t);
vec3 operator*(float t, const vec3 &v);

vec3 operator/(vec3 v, float t);

float dot(const vec3 &u, const vec3 &v);

vec3 cross(const vec3 &u, const vec3 &v);

vec3 unit_vector(vec3 v);

# endif // VEC3_H_
