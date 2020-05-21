#ifndef HITTABLE_H_
#define HITTABLE_H_

#include "ray.h"
#include "vec3.h"

class material;

struct hit_record {
  point3 p;
  vec3 normal;
  material *m;
  double t;
  bool front_face;

  void set_face_normal(const ray &r, const vec3 &outward_normal);
};

class hittable {
public:
  virtual ~hittable() {}
  virtual bool hit(const ray &r, double t_min, double t_max,
                   hit_record &rec) const = 0;
};
#endif // HITTABLE_H_
