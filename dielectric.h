#ifndef DIELECTRIC_H_
#define DIELECTRIC_H_

#include "material.h"

#include "color.h"
#include "hittable.h"
#include "ray.h"

class dielectric : public material {
public:
  dielectric(float ri);
  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override;

  static constexpr float AIR_REFRACTION_INDEX = 1.0;
  static constexpr float LIGHT_GLASS_REFRACTION_INDEX = 1.3;
  static constexpr float GLASS_REFRACTION_INDEX = 1.5;
  static constexpr float DENSE_GLASS_REFRACTION_INDEX = 1.7;
  static constexpr float DIAMOND_REFRACTION_INDEX = 2.4;

private:
  float refraction_index_;

  static float schlick(float cosine, float refraction_index);
};

#endif // DIELECTRIC_H_
