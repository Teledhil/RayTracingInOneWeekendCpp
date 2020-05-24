#include "color.h"

#include "utility.h"

void write_color(std::ostream &stream, const color &pixel,
                 int samples_per_pixel) {

  float r = pixel.x();
  float g = pixel.y();
  float b = pixel.z();

  // Divide the color total by the number of samples and gamma-correct it.
  // gamma = 2.0 --> square root.
  float scale = 1.0 / samples_per_pixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);

  stream << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
         << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
         << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << std::endl;
}
