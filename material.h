#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class material {
public:
  virtual ~material() {}
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       color &attenuation, ray &scattered) const = 0;
  virtual color emitted(double u __attribute__((unused)),
                        double v __attribute__((unused)),
                        const point3 &p __attribute__((unused))) const {
    return black;
  }
};
#endif // MATERIAL_H_
