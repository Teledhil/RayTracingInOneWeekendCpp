#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"

class lambertian : public material {
public:
  lambertian(const color &a);

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override;

private:
  color albedo_;
};
#endif // LAMBERTIAN_H_
