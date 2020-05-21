#ifndef DIELECTRIC_H_
#define DIELECTRIC_H_

#include "material.h"

#include "color.h"
#include "hittable.h"
#include "ray.h"

class dielectric : public material {
public:
  dielectric(double ri);
  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override;

  static constexpr double AIR_REFRACTION_INDEX = 1.0;
  static constexpr double LIGHT_GLASS_REFRACTION_INDEX = 1.3;
  static constexpr double GLASS_REFRACTION_INDEX = 1.5;
  static constexpr double DENSE_GLASS_REFRACTION_INDEX = 1.7;
  static constexpr double DIAMOND_REFRACTION_INDEX = 2.4;

private:
  double refraction_index_;

  static double schlick(double cosine, double refraction_index);
};

#endif // DIELECTRIC_H_
