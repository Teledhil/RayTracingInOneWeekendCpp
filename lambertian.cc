#include "lambertian.h"

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

lambertian::lambertian(const color &a) : albedo_(a) {}

bool lambertian::scatter(const ray &r_in, const hit_record &rec,
                         color &attenuation, ray &scattered) const {
  vec3 scatter_direction = rec.normal + vec3::random_unit_vector();

  scattered = ray(rec.p, scatter_direction);

  // TODO: make darker colors to have lower albedo values.
  attenuation = albedo_;

  return true;
}
