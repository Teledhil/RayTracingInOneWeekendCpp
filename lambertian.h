#pragma once

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "texture.h"
#include "vec3.h"

namespace rtx {
class lambertian : public material {
public:
  lambertian(const texture *a) : albedo_(a) {}
  ~lambertian() { delete albedo_; }

  bool scatter(const ray &r_in __attribute__((unused)), const hit_record &rec,
               color &attenuation, ray &scattered,
               rtx::random &ran) const override {
    vec3 scatter_direction = rec.normal + ran.random_unit_vector();
    while (scatter_direction == vec3(0, 0, 0)) {
      scatter_direction = rec.normal + ran.random_unit_vector();
    }

    // scattered = ray(rec.p, scatter_direction);
    scattered.origin(rec.p);
    scattered.direction(scatter_direction);

    // TODO: make darker colors to have lower albedo values.
    attenuation = albedo_->value(rec.u, rec.v, rec.p);

    return true;
  }

private:
  const texture *albedo_;
};
} // namespace rtx
