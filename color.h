#pragma once

#include <ostream>

#include "utility.h"
#include "vec3.h"

namespace rtx {
void write_color(std::ostream &stream, const color &pixel,
                 int samples_per_pixel) {

  float r = pixel.x();
  float g = pixel.y();
  float b = pixel.z();

  float scale = 1.0 / samples_per_pixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);

  stream << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
         << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
         << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << std::endl;
}

static const color white(1.0, 1.0, 1.0);
static const color black(0.0, 0.0, 0.0);
static const color blue(0.5, 0.7, 1.0);
static const color red(1.0, 0, 0);

} // namespace rtx
