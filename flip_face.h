#ifndef FLIP_FACE_H_
#define FLIP_FACE_H_

#include "aabb.h"
#include "hittable.h"
#include "ray.h"

namespace rtx {
class flip_face : public hittable {
public:
  flip_face(hittable * h) : h_(h) {}
  ~flip_face() { delete h_; }

  bool hit(const ray &r, float t_min, float t_max, hit_record &rec)
      const override {
    if (!h_->hit(r, t_min, t_max, rec)) {
      return false;
    }

    rec.front_face = !rec.front_face;
    return true;
  }

  bool bounding_box(float t0, float t1, aabb &output_box) const override {
    return h_->bounding_box(t0, t1, output_box);
  }

  private:
    hittable *h_;
  };
  }    // namespace rtx
#endif // FLIP_FACE_H_
