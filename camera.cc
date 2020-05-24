#include "camera.h"

#include <iostream>

#include "ray.h"
#include "utility.h"
#include "vec3.h"

camera::camera(point3 lookfrom, point3 lookat, vec3 vup, float vertical_fov,
               float aperture, float focus_distance)
    : origin_(lookfrom) {

  float theta = degrees_to_radians(vertical_fov);
  float h = tan(theta / 2);
  float viewport_height = 2 * h;
  float viewport_width = ASPECT_RATIO * viewport_height;

  w_ = unit_vector(lookfrom - lookat);
  u_ = unit_vector(cross(vup, w_));
  v_ = cross(w_, u_);

  horizontal_ = focus_distance * viewport_width * u_;
  vertical_ = focus_distance * viewport_height * v_;
  lower_left_corner_ =
      origin_ - horizontal_ / 2 - vertical_ / 2 - focus_distance * w_;

  lens_radius_ = aperture / 2;
}

ray camera::get_ray(float u, float v) const {
  vec3 rd = lens_radius_ * vec3::random_in_unit_disk();
  vec3 offset = u_ * rd.x() + v_ * rd.y();
  return ray(origin_ + offset, lower_left_corner_ + u * horizontal_ +
                                   v * vertical_ - origin_ - offset);
}
