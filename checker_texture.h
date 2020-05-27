#ifndef CHECKER_TEXTURE_H_
#define CHECKER_TEXTURE_H_

#include <math.h>

#include "texture.h"

class checker_texture : public texture {
public:
  checker_texture(const texture *t0, const texture *t1) : even(t0), odd(t1) {}
  ~checker_texture() {
    delete even;
    delete odd;
  }

  color value(float u, float v, const point3 &p) const override {
    float sines =
        std::sin(10 * p.x()) * std::sin(10 * p.y()) * std::sin(10 * p.z());
    if (sines < 0) {
      return odd->value(u, v, p);
    } else {
      return even->value(u, v, p);
    }
  }

private:
  const texture *even;
  const texture *odd;
};
#endif // CHECKER_TEXTURE_H_
