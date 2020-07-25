#pragma once

#include "texture.h"

#include "vec3.h"

namespace rtx {
class solid_color : public texture {
public:
  solid_color(const color &c) : color_value_(c) {}

  solid_color(float red, float green, float blue)
      : solid_color(color(red, green, blue)) {}

  color value(float u __attribute__((unused)), float v __attribute__((unused)),
              const point3 &p __attribute__((unused))) const override {
    return color_value_;
  }

private:
  const color color_value_;
};

} // namespace rtx
