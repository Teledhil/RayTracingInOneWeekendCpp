#ifndef YZ_RECT_H_
#define YZ_RECT_H_

#include "aabb.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

namespace rtx {
class yz_rect : public hittable {

public:
  yz_rect(float x, float y0, float y1, float z0, float z1, material *m)
      : x_(x), y0_(y0), y1_(y1), z0_(z0), z1_(z1), m_(m) {
    if (y1 < y0) {
      std::swap(y0, y1);
      // std::cerr << "Bad y0=" << y0 << ", y1=" << y1 << std::endl;
    }
    if (z1 < z0) {
      std::swap(z0, z1);
      // std::cerr << "Bad z0=" << z0 << ", z1=" << z1 << std::endl;
    }
  }
  ~yz_rect() { delete m_; }

  bool hit(const ray &r, float t_min, float t_max,
           hit_record &rec) const override {

    if (0 == r.direction().x()) {
      return false;
    }
    float t = (x_ - r.origin().x()) / r.direction().x();

    if (t < t_min || t > t_max) {
      return false;
    }

    float y = r.origin().y() + t * r.direction().y();
    float z = r.origin().z() + t * r.direction().z();

    if (y < y0_ || y > y1_ || z < z0_ || z > z1_) {
      return false;
    }

    rec.u = (y - y0_) / (y1_ - y0_);
    rec.v = (z - z0_) / (z1_ - z0_);
    rec.t = t;
    vec3 outward_normal = vec3(1, 0, 0);
    rec.set_face_normal(r, outward_normal);
    rec.m = m_;
    rec.p = r.at(t);

    return true;
  }

  bool bounding_box(float t0 __attribute__((unused)),
                    float t1 __attribute__((unused)),
                    aabb &output_box) const override {

    output_box =
        aabb(point3(x_ - 0.0001, y0_, z0_), point3(x_ + 0.0001, y1_, z1_));
    return true;
  }

private:
  float x_;
  float y0_;
  float y1_;
  float z0_;
  float z1_;
  material *m_;
};

} // namespace rtx
#endif // YZ_RECT_H_
