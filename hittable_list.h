#ifndef HITTABLE_LIST_H_
#define HITTABLE_LIST_H_

#include <vector>

#include "hittable.h"

class hittable_list {
public:
  hittable_list() : objects_() {}
  ~hittable_list() {
    for (auto o : objects_) {
      delete o;
    }
  }

  void add(hittable *h) {
    objects_.emplace_back(h);
  }

  bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const {

    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (auto o : objects_) {
      if (o->hit(r, t_min, t_max, temp_rec)) {
        hit_anything = true;
        if (temp_rec.t < closest_so_far) {
          closest_so_far = temp_rec.t;
          rec = temp_rec;
        }
      }
    }
    return hit_anything;
  }

private:
  std::vector<hittable *> objects_;
};

#endif // HITTABLE_LIST_H_
