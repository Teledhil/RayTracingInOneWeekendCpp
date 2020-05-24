#include "hittable_list.h"
#include "hittable.h"

hittable_list::hittable_list() : objects_() {}

hittable_list::~hittable_list() {
  for (auto o : objects_) {
    delete o;
  }
}

bool hittable_list::hit(const ray &r, float t_min, float t_max,
                        hit_record &rec) const {


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
