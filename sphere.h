#ifndef SPHERE_H_
#define SPHERE_H_

#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

class sphere : public hittable {
public:
  // sphere();
  sphere(point3 c, float r, material *m);
  ~sphere();

  bool hit(const ray &r, float t_min, float t_max,
           hit_record &rec) const override;

  const point3 &center() const { return center_; }
  float radius() const { return radius_; }

private:
  point3 center_;
  float radius_;
  material *material_;
};

#endif // SPHERE_H_
