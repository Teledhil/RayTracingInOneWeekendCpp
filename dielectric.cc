#include "dielectric.h"

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "utility.h"
#include "vec3.h"

dielectric::dielectric(double ri) : refraction_index_(ri) {}

bool dielectric::scatter(const ray &r_in, const hit_record &rec,
                         color &attenuation, ray &scattered) const {


  double etai_over_etat;
  if (rec.front_face) {
    etai_over_etat = AIR_REFRACTION_INDEX / refraction_index_;
  } else {
    etai_over_etat = refraction_index_ * AIR_REFRACTION_INDEX;
  }

  vec3 unit_direction = unit_vector(r_in.direction());
  double cos_theta = fmin(1.0, dot(-unit_direction, rec.normal));
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  if ((etai_over_etat * sin_theta > 1.0) ||
      (random_double() < schlick(cos_theta, etai_over_etat))) {
    // Reflection: Either the incidence angle is to great or the specular
    // reflection happens.
    vec3 reflected = vec3::reflect(unit_direction, rec.normal);
    scattered = ray(rec.p, reflected);
  } else {
    // Refraction
    vec3 refracted = vec3::refract(unit_direction, rec.normal, etai_over_etat);
    scattered = ray(rec.p, refracted);
  }
  attenuation = white;

  return true;
}

double dielectric::schlick(double cosine, double refraction_index) {
  // https://en.wikipedia.org/wiki/Schlick%27s_approximation
  //
  // TODO: Only works when the other material is air?
  //
  double r0 = (1 - refraction_index) / (1 + refraction_index);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow(1 - cosine, 5);
}
