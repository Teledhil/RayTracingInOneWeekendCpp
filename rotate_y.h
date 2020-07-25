#ifndef ROTATE_Y_H_
#define ROTATE_Y_H_

#include <math.h>

#include "aabb.h"
#include "hittable.h"
#include "random.h"
#include "ray.h"
#include "utility.h"
#include "vec3.h"

namespace rtx {
class rotate_y : public hittable {
public:
  rotate_y(hittable *h, float angle) : h_(h) {
    float radians = degrees_to_radians(angle);
    sin_theta_ = sin(radians);
    cos_theta_ = cos(radians);
    has_box_ = h_->bounding_box(0, 1, bbox_);

    point3 min(infinity, infinity, infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          float x = i * bbox_.max().x() + (1 - i) * bbox_.min().x();
          float y = j * bbox_.max().y() + (1 - j) * bbox_.min().y();
          float z = k * bbox_.max().z() + (1 - k) * bbox_.min().z();

          float newx = cos_theta_ * x + sin_theta_ * z;
          float newz = -sin_theta_ * x + cos_theta_ * z;

          vec3 tester(newx, y, newz);

          for (int c = 0; c < 3; c++) {
            min[c] = fmin(min[c], tester[c]);
            max[c] = fmax(max[c], tester[c]);
          }
        }
      }
    }

    bbox_ = aabb(min, max);
  }
  ~rotate_y() { delete h_; }

  bool hit(const ray &r, float t_min, float t_max, hit_record &rec,
           rtx::random &ran) const override {

    point3 origin = r.origin();
    point3 direction = r.direction();

    origin[0] = cos_theta_ * r.origin()[0] - sin_theta_ * r.origin()[2];
    origin[2] = sin_theta_ * r.origin()[0] + cos_theta_ * r.origin()[2];

    direction[0] =
        cos_theta_ * r.direction()[0] - sin_theta_ * r.direction()[2];
    direction[2] =
        sin_theta_ * r.direction()[0] + cos_theta_ * r.direction()[2];

    ray ray_rotated(origin, direction);

    if (!h_->hit(ray_rotated, t_min, t_max, rec, ran)) {
      return false;
    }

    point3 p = rec.p;
    vec3 normal = rec.normal;

    p[0] = cos_theta_ * rec.p[0] + sin_theta_ * rec.p[2];
    p[2] = -sin_theta_ * rec.p[0] + cos_theta_ * rec.p[2];

    normal[0] = cos_theta_ * rec.normal[0] + sin_theta_ * rec.normal[2];
    normal[2] = -sin_theta_ * rec.normal[0] + cos_theta_ * rec.normal[2];

    rec.p = p;
    rec.set_face_normal(ray_rotated, normal);

    return true;
  }

  bool bounding_box(float t0 __attribute__((unused)),
                    float t1 __attribute__((unused)),
                    aabb &output_box) const override {
    output_box = bbox_;
    return has_box_;
  }

private:
  hittable *h_;
  float sin_theta_;
  float cos_theta_;
  bool has_box_;
  aabb bbox_;
};
} // namespace rtx
#endif // ROTATE_Y_H_
