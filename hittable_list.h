#pragma once

#include <vector>

#include "aabb.h"
#include "bvh.h"
#include "hittable.h"
#include "random.h"

namespace rtx {
class hittable_list {
public:
  hittable_list() : objects_() {}
  ~hittable_list() {
    for (auto o : objects_) {
      delete o;
    }
  }

  void add(hittable *h) { objects_.emplace_back(h); }

  void add(hittable_list &other) {
    for (auto &o : other.objects_) {
      objects_.emplace_back(o);
    }
    other.objects_.clear();
  }

  bool hit(const ray &r, float t_min, float t_max, hit_record &rec,
           rtx::random &ran) const {

    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (auto o : objects_) {
      if (o->hit(r, t_min, t_max, temp_rec, ran)) {
        hit_anything = true;
        if (temp_rec.t < closest_so_far) {
          closest_so_far = temp_rec.t;
          rec = temp_rec;
        }
      }
    }
    return hit_anything;
  }

  bool bounding_box(float t0, float t1, aabb &output_box) const {
    if (objects_.empty()) {
      return false;
    }

    aabb temp_box;
    bool first_box = true;
    for (const auto o : objects_) {
      if (!o->bounding_box(t0, t1, temp_box)) {
        return false;
      }
      output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
      first_box = false;
    }
    return true;
  }

  void optimize_bvh(rtx::random &r) {
    bvh_node *bvh = new bvh_node(objects_, 0, objects_.size(), 0.0, 1.0, r);
    objects_.clear();
    objects_.emplace_back(bvh);
  }

  hittable *eject_bvh() {
    hittable *b = objects_[0];
    objects_.clear();
    return b;
  }

public:
  std::vector<hittable *> objects_;
};

} // namespace rtx
