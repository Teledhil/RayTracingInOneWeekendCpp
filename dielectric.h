#ifndef DIELECTRIC_H_
#define DIELECTRIC_H_

#include "material.h"

#include "color.h"
#include "hittable.h"
#include "random.h"
#include "ray.h"
#include "utility.h"
#include "vec3.h"

namespace rtx {
class dielectric : public material {
public:
  dielectric(float ri) : refraction_index_(ri) {}

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered, rtx::random &ran) const override {

    float etai_over_etat;
    if (rec.front_face) {
      etai_over_etat = AIR_REFRACTION_INDEX / refraction_index_;
    } else {
      etai_over_etat = refraction_index_ * AIR_REFRACTION_INDEX;
    }

    vec3 unit_direction = unit_vector(r_in.direction());
    float cos_theta = fmin(1.0, dot(-unit_direction, rec.normal));
    float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    if ((etai_over_etat * sin_theta > 1.0) ||
        (ran.random_float() < schlick(cos_theta, etai_over_etat))) {
      // Reflection: Either the incidence angle is to great or the specular
      // reflection happens.
      vec3 reflected = reflect(unit_direction, rec.normal);
      // scattered = ray(rec.p, reflected);
      scattered.origin(rec.p);
      scattered.direction(reflected);
    } else {
      // Refraction
      vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
      // scattered = ray(rec.p, refracted);
      scattered.origin(rec.p);
      scattered.direction(refracted);
    }
    attenuation = white;

    return true;
  }

  static constexpr float AIR_REFRACTION_INDEX = 1.0;
  static constexpr float LIGHT_GLASS_REFRACTION_INDEX = 1.3;
  static constexpr float GLASS_REFRACTION_INDEX = 1.5;
  static constexpr float DENSE_GLASS_REFRACTION_INDEX = 1.7;
  static constexpr float DIAMOND_REFRACTION_INDEX = 2.4;

private:
  float refraction_index_;

  static float schlick(float cosine, float refraction_index) {
    // https://en.wikipedia.org/wiki/Schlick%27s_approximation
    //
    // TODO: Only works when the other material is air?
    //
    float r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
  }
};

} // namespace rtx
#endif // DIELECTRIC_H_
