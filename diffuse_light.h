#ifndef DIFFUSE_LIGHT_H_
#define DIFFUSE_LIGHT_H_

#include "hittable.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "texture.h"
#include "vec3.h"

namespace rtx {
class diffuse_light : public material {
public:
  diffuse_light(texture *e) : emit_(e) {}

  bool scatter(const ray &r_in __attribute__((unused)),
               const hit_record &rec __attribute__((unused)),
               color &attenuation __attribute__((unused)),
               ray &scattered __attribute__((unused)),
               rtx::random &ran __attribute__((unused))) const override {
    return false;
  }

  color emitted(double u, double v, const point3 &p) const override {

    return emit_->value(u, v, p);
  }

private:
  texture *emit_;
};
} // namespace rtx
#endif // DIFFUSE_LIGHT_H_
