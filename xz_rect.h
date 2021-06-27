#pragma once

#include <utility>

#include "aabb.h"
#include "hittable.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "vec3.h"

namespace rtx {
class xz_rect : public hittable {
public:
  xz_rect(float x0, float x1, float y, float z0, float z1, material *m)
      : x0_(x0), x1_(x1), y_(y), z0_(z0), z1_(z1), m_(m) {

    if (x1 < x0) {
      std::swap(x0, x1);
    }
    if (z1 < z0) {
      std::swap(z0, z1);
    }
  }
  ~xz_rect() {
    // delete m_;
  }

  bool hit(const ray &r, float t_min, float t_max, hit_record &rec,
           rtx::random &ran __attribute__((unused))) const override {

    if (0 == r.direction().y()) {
      return false;
    }
    float t = (y_ - r.origin().y()) / r.direction().y();

    if (t < t_min || t > t_max) {
      return false;
    }

    float x = r.origin().x() + t * r.direction().x();
    float z = r.origin().z() + t * r.direction().z();

    if (x < x0_ || x > x1_ || z < z0_ || z > z1_) {
      return false;
    }

    rec.u = (x - x0_) / (x1_ - x0_);
    rec.v = (z - z0_) / (z1_ - z0_);
    rec.t = t;
    vec3 outward_normal = vec3(0, 1, 0);
    rec.set_face_normal(r, outward_normal);
    rec.m = m_;
    rec.p = r.at(t);

    return true;
  }

  bool bounding_box(float t0 __attribute__((unused)),
                    float t1 __attribute__((unused)),
                    aabb &output_box) const override {

    output_box =
        aabb(point3(x0_, y_ - 0.0001, z0_), point3(x1_, y_ + 0.0001, z1_));
    return true;
  }

private:
  float x0_;
  float x1_;
  float y_;
  float z0_;
  float z1_;
  material *m_;
};
} // namespace rtx
