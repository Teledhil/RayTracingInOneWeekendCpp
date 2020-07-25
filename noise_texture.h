#pragma once

#include <math.h>

#include "perlin.h"
#include "random.h"
#include "texture.h"
#include "vec3.h"

namespace rtx {
class noise_texture : public texture {
public:
  noise_texture(float sc, rtx::random &r) : noise_(r), scale_(sc) {}
  color value(float u __attribute__((unused)), float v __attribute__((unused)),
              const point3 &p) const override {
    return color(1, 1, 1) * 0.5 *
           (1.0 + sin(scale_ * p.y() + 10 * noise_.turbulence(scale_ * p)));
  }

private:
  perlin noise_;
  float scale_;
};
} // namespace rtx
