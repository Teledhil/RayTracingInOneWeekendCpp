#ifndef IOSTROPIC_H_
#define IOSTROPIC_H_

#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "texture.h"
#include "vec3.h"

namespace rtx {
class isotropic : public material {
public:
  isotropic(texture *t) : albedo_(t) {}
  ~isotropic() { delete albedo_; }

  bool scatter(const ray &r_in __attribute__((unused)), const hit_record &rec,
               color &attenuation, ray &scattered) const override {

    scattered.origin(rec.p);
    scattered.direction(random_in_unit_sphere());
    attenuation = albedo_->value(rec.u, rec.v, rec.p);

    return true;
  }

private:
  const texture *albedo_;
};
} // namespace rtx
#endif // IOSTROPIC_H_
