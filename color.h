#ifndef COLOR_H_
#define COLOR_H_

#include <iostream>

#include "utility.h"
#include "vec3.h"

namespace rtx {
void write_color(std::ostream &stream, const color &pixel,
                 int samples_per_pixel) {

  float r = pixel.x();
  float g = pixel.y();
  float b = pixel.z();

  // if (pixel.is_bad()) {
  //  std::cerr << "Bad pixel" << std::endl;
  //}

  // Divide the color total by the number of samples and gamma-correct it.
  // gamma = 2.0 --> square root.
  // r = sqrt(r / float(samples_per_pixel)) * 255.99f;
  // g = sqrt(g / float(samples_per_pixel)) * 255.99f;
  // b = sqrt(b / float(samples_per_pixel)) * 255.99f;

  // stream << static_cast<int>(r) << ' ' << static_cast<int>(g) << ' '
  //       << static_cast<int>(b) << std::endl;

  float scale = 1.0 / samples_per_pixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);

  // if (r != r) {
  //  std::cerr << "bad r" << std::endl;
  //}
  // if (g != g) {
  //  std::cerr << "bad g" << std::endl;
  //}
  // if (b != b) {
  //  std::cerr << "bad b" << std::endl;
  //}

  // int a_r = static_cast<int>(256 * clamp(r, 0.0, 0.999));
  // int a_g = static_cast<int>(256 * clamp(g, 0.0, 0.999));
  // int a_b = static_cast<int>(256 * clamp(b, 0.0, 0.999));

  // if (a_r < 0) {
  //  std::cerr << "bad a_r" << std::endl;
  //}
  // if (a_g < 0) {
  //  std::cerr << "bad a_g" << std::endl;
  //}
  // if (a_b < 0) {
  //  std::cerr << "bad a_b" << std::endl;
  //}

  // stream << a_r << ' ' << a_g << ' ' << a_b << std::endl;

  stream << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
         << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
         << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << std::endl;
}

  static const color white(1.0, 1.0, 1.0);
  static const color black(0.0, 0.0, 0.0);
  static const color blue(0.5, 0.7, 1.0);
  static const color red(1.0, 0, 0);

  }    // namespace rtx
#endif // COLOR_H_
