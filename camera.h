#ifndef CAMERA_H_
#define CAMERA_H_

#include "ray.h"
#include "vec3.h"

class camera {
public:
  camera(point3 lookfrom, point3 lookat, vec3 vup, double vertical_fov,
         double aperture, double focus_distance);

  ray get_ray(double u, double v) const;

  int getWidth() const { return WIDTH; }
  int getHeight() const { return HEIGHT; }

private:
  static constexpr int WIDTH = 3840;
  static constexpr int HEIGHT = 2160;
  static constexpr double ASPECT_RATIO = double(WIDTH) / double(HEIGHT);
  static constexpr double FOCAL_LENGTH = 1.0;

  point3 origin_;
  point3 lower_left_corner_;
  vec3 horizontal_;
  vec3 vertical_;
  vec3 u_, v_, w_;
  double lens_radius_;
};
#endif // CAMERA_H_
