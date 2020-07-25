#pragma once

#include "hittable.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "texture.h"
#include "vec3.h"

namespace rtx {
class isotropic : public material {
public:
  isotropic(texture *t) : albedo_(t) {}
  ~isotropic() { delete albedo_; }

  bool scatter(const ray &r_in __attribute__((unused)), const hit_record &rec,
               color &attenuation, ray &scattered,
               rtx::random &ran) const override {

    scattered.origin(rec.p);
    scattered.direction(ran.random_in_unit_sphere());
    attenuation = albedo_->value(rec.u, rec.v, rec.p);

    return true;
  }

private:
  const texture *albedo_;
};
} // namespace rtx
