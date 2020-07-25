#pragma once

#include "aabb.h"
#include "hittable.h"
#include "isotropic.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "texture.h"

namespace rtx {
class constant_medium : public hittable {
public:
  constant_medium(hittable *h, texture *t, float density)
      : h_(h), t_(new isotropic(t)), neg_inv_density_(-1 / density) {}
  ~constant_medium() {
    delete h_;
    delete t_;
  }

  bool hit(const ray &r, float t_min, float t_max, hit_record &rec,
           rtx::random &ran) const override {

    hit_record rec1;
    hit_record rec2;

    if (!h_->hit(r, -infinity, infinity, rec1, ran)) {
      return false;
    }

    if (!h_->hit(r, rec1.t + 0.0001, infinity, rec2, ran)) {
      return false;
    }

    if (rec1.t < t_min) {
      rec1.t = t_min;
    }
    if (rec2.t > t_max) {
      rec2.t = t_max;
    }

    if (rec1.t >= rec2.t) {
      // TODO: Can this happen?
      return false;
    }

    if (rec1.t < 0) {
      return false;
    }

    const float ray_length = r.direction().length();
    const float distance_inside_hittable = (rec2.t - rec1.t) * ray_length;
    const float hit_distance = neg_inv_density_ * log(ran.random_float());

    if (hit_distance > distance_inside_hittable) {
      return false;
    }

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);
    rec.normal = vec3(1, 0, 0); // Arbitrary
    rec.front_face = true;      // Arbitrary
    rec.m = t_;

    return true;
  }

  bool bounding_box(float t0, float t1, aabb &output_box) const override {
    return h_->bounding_box(t0, t1, output_box);
  }

private:
  hittable *h_;
  material *t_;
  float neg_inv_density_;
};
} // namespace rtx
