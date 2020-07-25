#pragma once

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "vec3.h"

namespace rtx {
class metal : public material {
public:
  metal(const color &a, float f) : albedo_(a), fuzziness_(f > 1 ? 1 : f) {}

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered, rtx::random &ran) const override {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);

    // scattered = ray(rec.p, reflected + fuzziness_ * random_in_unit_sphere());
    scattered.origin(rec.p);
    scattered.direction(reflected + fuzziness_ * ran.random_in_unit_sphere());
    attenuation = albedo_;

    bool from_outside = dot(scattered.direction(), rec.normal) > 0;
    return from_outside;
  }

private:
  color albedo_;
  float fuzziness_;
};
} // namespace rtx
