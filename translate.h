#ifndef TRANSLATE_H_
#define TRANSLATE_H_

#include "aabb.h"
#include "hittable.h"
#include "random.h"
#include "ray.h"
#include "vec3.h"

namespace rtx {
class translate : public hittable {
public:
  translate(hittable *h, const vec3 &displacement)
      : h_(h), displacement_(displacement) {}
  ~translate() { delete h_; }

  bool hit(const ray &r, float t_min, float t_max, hit_record &rec,
           rtx::random &ran) const override {
    ray ray_moved(r.origin() - displacement_, r.direction());

    if (!h_->hit(ray_moved, t_min, t_max, rec, ran)) {
      return false;
    }

    rec.p += displacement_;
    rec.set_face_normal(ray_moved, rec.normal);

    return true;
  }

  bool bounding_box(float t0, float t1, aabb &output_box) const override {
    if (!h_->bounding_box(t0, t1, output_box)) {
      return false;
    }

    output_box = aabb(output_box.min() + displacement_,
                      output_box.max() + displacement_);
    return true;
  }

private:
  hittable *h_;
  vec3 displacement_;
};

} // namespace rtx
#endif // TRANSLATE_H_
