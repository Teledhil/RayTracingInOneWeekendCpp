#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "vec3.h"

class texture {
public:
  virtual ~texture() {}
  virtual color value(float u, float v, const point3 &p) const = 0;
};

#endif // TEXTURE_H_
