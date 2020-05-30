#ifndef HITTABLE_H_
#define HITTABLE_H_

#include "ray.h"
#include "vec3.h"

class material;

struct hit_record {
  point3 p;
  vec3 normal;
  material *m;
  float t;
  float u;
  float v;
  bool front_face;

  void set_face_normal(const ray &r, const vec3 &outward_normal) {
    if (dot(r.direction(), outward_normal) < 0) {
      // Ray is outside the sphere
      front_face = true;
      normal = outward_normal;
    } else {
      // Ray is inside the sphere
      front_face = false;
      normal = -1 * outward_normal;
    }
  }
};

class hittable {
public:
  virtual ~hittable() {}
  virtual bool hit(const ray &r, float t_min, float t_max,
                   hit_record &rec) const = 0;
  virtual bool bounding_box(float t0, float t1, aabb &output_box) const = 0;
};
#endif // HITTABLE_H_
