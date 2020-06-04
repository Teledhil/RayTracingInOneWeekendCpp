#ifndef AABB_H_
#define AABB_H_

#include "ray.h"
#include "vec3.h"

namespace rtx {
class aabb {
public:
  aabb() : min_(), max_() {}
  aabb(const point3 &a, const point3 &b) : min_(a), max_(b) {}

  point3 min() const { return min_; }
  point3 max() const { return max_; }

  bool hit(const ray &r, float t_min, float t_max) const {

    vec3 inverse_direction = inv(r.direction());
    vec3 t0 = (min_ - r.origin()) * inverse_direction;
    vec3 t1 = (max_ - r.origin()) * inverse_direction;

    t_min = std::max(t_min, rtx::max(rtx::min(t0, t1)));
    t_max = std::min(t_max, rtx::min(rtx::max(t0, t1)));

    return t_max > t_min;
  }

private:
  point3 min_;
  point3 max_;
};

aabb surrounding_box(const aabb &box0, const aabb &box1) {

  point3 small = rtx::min(box0.min(), box1.min());
  point3 big = rtx::max(box0.max(), box1.max());

  return aabb(small, big);
}
} // namespace rtx
#endif // AABB_H_
