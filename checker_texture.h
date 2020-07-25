#pragma once

#include <math.h>

#include "texture.h"

namespace rtx {
class checker_texture : public texture {
public:
  checker_texture(const texture *t0, const texture *t1) : even_(t0), odd_(t1) {}
  ~checker_texture() {
    delete even_;
    delete odd_;
  }

  color value(float u, float v, const point3 &p) const override {
    float sines =
        std::sin(10 * p.x()) * std::sin(10 * p.y()) * std::sin(10 * p.z());
    if (sines < 0) {
      return odd_->value(u, v, p);
    } else {
      return even_->value(u, v, p);
    }
  }

private:
  const texture *even_;
  const texture *odd_;
};
} // namespace rtx
