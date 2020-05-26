#ifndef AABB_H_
#define AABB_H_

#include "ray.h"
#include "vec3.h"

class aabb {
public:
  aabb() : min_(), max_() {}
  aabb(const point3 &a, const point3 &b) : min_(a), max_(b) {}

  point3 min() const { return min_; }
  point3 max() const { return max_; }

  bool hit(const ray &r, float t_min, float t_max) const {

    for (int a = 0; a < 3; ++a) {
      float inverse_direction = 1.0f / r.direction()[a];

      float t0 = (min_[a] - r.origin()[a]) * inverse_direction;
      float t1 = (max_[a] - r.origin()[a]) * inverse_direction;

      if (inverse_direction < 0.0f) {
        std::swap(t0, t1);
      }

      t_min = t0 > t_min ? t0 : t_min;
      t_max = t1 < t_max ? t1 : t_max;

      if (t_max <= t_min) {
        return false;
      }
    }
    return true;
  }

private:
  point3 min_;
  point3 max_;
};

aabb surrounding_box(aabb box0, aabb box1) {
  point3 small(fmin(box0.min().x(), box1.min().x()),
               fmin(box0.min().y(), box1.min().y()),
               fmin(box0.min().z(), box1.min().z()));

  point3 big(fmax(box0.max().x(), box1.max().x()),
             fmax(box0.max().y(), box1.max().y()),
             fmax(box0.max().z(), box1.max().z()));

  return aabb(small, big);
}
#endif // AABB_H_
