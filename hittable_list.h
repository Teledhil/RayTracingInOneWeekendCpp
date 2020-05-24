#ifndef HITTABLE_LIST_H_
#define HITTABLE_LIST_H_

#include <vector>

#include "hittable.h"

class hittable_list {
public:
  hittable_list();
  ~hittable_list();

  void add(hittable *h) {
    objects_.emplace_back(h);
  }

  bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;

private:
  std::vector<hittable *> objects_;
};

#endif // HITTABLE_LIST_H_
