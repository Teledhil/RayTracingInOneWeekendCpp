#pragma once

#include <vector>

#include "color.h"
#include "texture.h"

#include "material.h"

#include "dielectric.h"
#include "diffuse_light.h"
#include "isotropic.h"
#include "lambertian.h"
#include "metal.h"

namespace rtx {
class MaterialManager {
  public:
    MaterialManager(): materials_() {}

    ~MaterialManager() {
      for (auto material : materials_) {
        delete material;
      }
      materials_.clear();
    }

    material *new_lambertian(const texture *a) {
      material *m = new lambertian(a);
      materials_.push_back(m);
      return m;
    }

    material *new_metal(const color &a, float f) {
      material *m = new metal(a, f);
      materials_.push_back(m);
      return m;
    }

    isotropic *new_isotropic(texture *t) {
      isotropic *m = new isotropic(t);
      materials_.push_back(m);
      return m;
    }

    material *new_diffuse_light(texture *e) {
      material *m = new diffuse_light(e);
      materials_.push_back(m);
      return m;
    }

    material *new_dielectric(float ri) {
      material *m = new dielectric(ri);
      materials_.push_back(m);
      return m;
    }

    material *new_glass() { return new_dielectric(1.5); }

  private:
    std::vector<material*> materials_;
};
} // namespace rtx
