#ifndef RAY_H_
#define RAY_H_

#include "vec3.h"

class ray {
public:
  ray() : origin_(), direction_() {}
  ray(const point3 &o, const vec3 &d) : origin_(o), direction_(d) {}
  ray(const ray &other)
      : origin_(other.origin_), direction_(other.direction_) {}

  const point3 &origin() const { return origin_; }
  const vec3 &direction() const { return direction_; }

  point3 at(double t) const { return origin_ + t * direction_; }

private:
  point3 origin_;
  vec3 direction_;
};

#endif // RAY_H_
