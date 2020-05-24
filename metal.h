#ifndef METAL_H_
#define METAL_H_

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"

class metal : public material {
public:
  metal(const color &a, float f);

  bool scatter(const ray &r_in, const hit_record &rec, color &attenuation,
               ray &scattered) const override;

private:
  color albedo_;
  float fuzziness_;
};
#endif // METAL_H_
