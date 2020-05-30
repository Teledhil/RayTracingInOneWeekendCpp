#ifndef XY_RECT_H_
#define XY_RECT_H_

#include "aabb.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

class xy_rect : public hittable {

public:
  xy_rect(float x0, float x1, float y0, float y1, float z, material *m)
      : x0_(x0), x1_(x1), y0_(y0), y1_(y1), z_(z), m_(m) {
    if (x1 < x0) {
      std::cerr << "Bad x0=" << x0 << ", x1=" << x1 << std::endl;
    }
    if (y1 < y0) {
      std::cerr << "Bad y0=" << y0 << ", y1=" << y1 << std::endl;
    }
  }
  ~xy_rect() { delete m_; }

  bool hit(const ray &r, float t_min, float t_max,
           hit_record &rec) const override {

    if (0 == r.direction().z()) {
      return false;
    }
    float t = (z_ - r.origin().z()) / r.direction().z();

    if (t < t_min || t > t_max) {
      return false;
    }

    float x = r.origin().x() + t * r.direction().x();
    float y = r.origin().y() + t * r.direction().y();

    if (x < x0_ || x > x1_ || y < y0_ || y > y1_) {
      return false;
    }

    rec.u = (x - x0_) / (x1_ - x0_);
    rec.v = (y - y0_) / (y1_ - y0_);
    rec.t = t;
    vec3 outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.m = m_;
    rec.p = r.at(t);

    return true;
  }

  bool bounding_box(float t0 __attribute__((unused)),
                    float t1 __attribute__((unused)),
                    aabb &output_box) const override {

    output_box =
        aabb(point3(x0_, y0_, z_ - 0.0001), point3(x1_, y1_, z_ + 0.0001));
    return true;
  }

private:
  float x0_;
  float x1_;
  float y0_;
  float y1_;
  float z_;
  material *m_;
};
#endif // XY_RECT_H_