#ifndef RAY_H_
#define RAY_H_

#include "vec3.h"

class ray {
public:
  ray() = default;
  ray(const point3 &o, const vec3 &d) : origin_(o), direction_(d) {}
  ray(const ray &) = default;
  ray(ray &&) = default;
  ~ray() = default;
  ray &operator=(const ray &) = delete;
  ray &operator=(ray &) = delete;

  const point3 &origin() const { return origin_; }
  const vec3 &direction() const { return direction_; }

  void origin(const point3 &o) { origin_ = o; }
  void direction(const vec3 &d) { direction_ = d; }

  point3 at(float t) const { return origin_ + t * direction_; }

private:
  point3 origin_;
  vec3 direction_;
};

#endif // RAY_H_
