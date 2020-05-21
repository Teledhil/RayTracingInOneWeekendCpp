#include "metal.h"

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

metal::metal(const color &a, double f)
    : albedo_(a), fuzziness_(f > 1 ? 1 : f) {}

bool metal::scatter(const ray &r_in, const hit_record &rec, color &attenuation,
                    ray &scattered) const {
  vec3 reflected = vec3::reflect(unit_vector(r_in.direction()), rec.normal);

  scattered =
      ray(rec.p, reflected + fuzziness_ * vec3::random_in_unit_sphere());
  attenuation = albedo_;

  bool from_outside = dot(scattered.direction(), rec.normal) > 0;
  return from_outside;
}
