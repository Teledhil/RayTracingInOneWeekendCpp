#ifndef CAMERA_H_
#define CAMERA_H_

#include "ray.h"
#include "vec3.h"

class camera {
public:
  camera(point3 lookfrom, point3 lookat, vec3 vup, float vertical_fov,
         float aperture, float focus_distance);

  ray get_ray(float u, float v) const;

  int getWidth() const { return WIDTH; }
  int getHeight() const { return HEIGHT; }

private:
  static constexpr int WIDTH = 1280;
  static constexpr int HEIGHT = 720;
  static constexpr float ASPECT_RATIO = float(WIDTH) / float(HEIGHT);
  static constexpr float FOCAL_LENGTH = 1.0;

  point3 origin_;
  point3 lower_left_corner_;
  vec3 horizontal_;
  vec3 vertical_;
  vec3 u_, v_, w_;
  float lens_radius_;
};
#endif // CAMERA_H_
