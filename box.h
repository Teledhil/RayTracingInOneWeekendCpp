#pragma once

#include <vector>

#include "aabb.h"
#include "flip_face.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "random.h"
#include "vec3.h"
#include "xy_rect.h"
#include "xz_rect.h"
#include "yz_rect.h"

namespace rtx {
class box : public hittable {
public:
  box(const point3 &p0, const point3 &p1, const std::vector<material *> &m)
      : box_min_(p0), box_max_(p1), sides_(), m_(m) {

    sides_.add(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), m_[0]));
    sides_.add(new flip_face(
        new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), m_[1])));

    sides_.add(new xz_rect(p0.x(), p1.x(), p0.y(), p0.z(), p1.z(), m_[2]));
    sides_.add(new flip_face(
        new xz_rect(p0.x(), p1.x(), p1.y(), p0.z(), p1.z(), m_[3])));

    sides_.add(new yz_rect(p0.x(), p0.y(), p0.y(), p0.z(), p1.z(), m_[4]));
    sides_.add(new flip_face(
        new yz_rect(p0.x(), p0.y(), p1.y(), p0.z(), p1.z(), m_[5])));
  }

  bool hit(const ray &r, float t_min, float t_max, hit_record &rec,
           rtx::random &ran) const override {
    return sides_.hit(r, t_min, t_max, rec, ran);
  }

  bool bounding_box(float t0 __attribute__((unused)),
                    float t1 __attribute__((unused)),
                    aabb &output_box) const override {
    output_box = aabb(box_min_, box_max_);
    return true;
  }

private:
  point3 box_min_;
  point3 box_max_;
  hittable_list sides_;
  std::vector<material *> m_;
};
} // namespace rtx
