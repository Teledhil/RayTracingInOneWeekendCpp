#include "hittable.h"

#include "ray.h"
#include "vec3.h"

void hit_record::set_face_normal(const ray &r, const vec3 &outward_normal) {
  if (dot(r.direction(), outward_normal) < 0) {
    // Ray is outside the sphere
    front_face = true;
    normal = outward_normal;
  } else {
    // Ray is inside the sphere
    front_face = false;
    normal = -outward_normal;
  }
}
