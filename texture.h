#pragma once

#include "vec3.h"

namespace rtx {

class texture {
public:
  virtual ~texture() {}
  virtual color value(float u, float v, const point3 &p) const = 0;
};
} // namespace rtx
