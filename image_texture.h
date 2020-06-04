#ifndef IMAGE_TEXTURE_H_
#define IMAGE_TEXTURE_H_

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "texture.h"
#include "utility.h"
#include "vec3.h"

namespace rtx {
class image_texture : public texture {
public:
  image_texture(const char *filename) {
    int components_per_pixel = BYTES_PER_PIXEL;

    data_ = stbi_load(filename, &width_, &height_, &components_per_pixel,
                      components_per_pixel);
    if (!data_) {
      std::cerr << "ERROR: Could not load texture image file " << filename
                << "." << std::endl;
      width_ = 0;
      height_ = 0;
    }

    bytes_per_scanline_ = BYTES_PER_PIXEL * width_;
  }

  ~image_texture() { delete data_; }

  color value(float u, float v,
              const point3 &p __attribute__((unused))) const override {
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates.

    int i = static_cast<int>(u * width_);
    int j = static_cast<int>(v * height_);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    // TODO: This shouldn't happen anyway.
    if (i >= width_)
      i = width_ - 1;
    if (j >= height_)
      j = height_ - 1;

    auto pixel = data_ + j * bytes_per_scanline_ + i * BYTES_PER_PIXEL;

    return color(COLOR_SCALE * pixel[0], COLOR_SCALE * pixel[1],
                 COLOR_SCALE * pixel[2]);
  }

private:
  static constexpr int BYTES_PER_PIXEL = 3;
  static constexpr float COLOR_SCALE = 1.0 / 255;

  unsigned char *data_;
  int width_;
  int height_;
  int bytes_per_scanline_;
};

} // namespace rtx
#endif // IMAGE_TEXTURE_H_
