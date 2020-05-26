#ifndef BVH_H_
#define BVH_H_

#include <algorithm>
#include <iostream>
#include <vector>

#include "aabb.h"
#include "hittable.h"
#include "ray.h"

bool box_compare(const hittable *a, const hittable *b, int axis) {
  aabb box_a;
  aabb box_b;

  if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    std::cerr << "No bounding box in bvh_node constructor.\n";

  return box_a.min()[axis] < box_b.min()[axis];
}

bool box_x_compare(const hittable *a, const hittable *b) {
  return box_compare(a, b, 0);
}

bool box_y_compare(const hittable *a, const hittable *b) {
  return box_compare(a, b, 1);
}

bool box_z_compare(const hittable *a, const hittable *b) {
  return box_compare(a, b, 2);
}

class bvh_node : public hittable {
public:
  bvh_node(std::vector<hittable *> &objects, size_t start, size_t end, float t0,
           float t1) {

    // pick a random axis to split objets left and right
    // int axis = random_int(0, 2);
    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare
                                  : (axis == 1) ? box_z_compare : box_y_compare;

    // Build the binary tree
    size_t object_span = end - start;

    if (object_span == 1) {
      // Both nodes contain the same object so no leafs are null.
      // TODO: unhittable
      left = objects[start];
      right = objects[start];
    } else if (object_span == 2) {
      if (comparator(objects[start], objects[start + 1])) {
        left = objects[start];
        right = objects[start + 1];
      } else {
        left = objects[start + 1];
        right = objects[start];
      }
    } else {
      std::sort(objects.begin() + start, objects.begin() + end, comparator);
      size_t mid = start + object_span / 2;
      left = new bvh_node(objects, start, mid, t0, t1);
      right = new bvh_node(objects, mid, end, t0, t1);
    }


    // Make sure children have bounding boxes.
    aabb left_box;
    aabb right_box;

    if (!left->bounding_box(t0, t1, left_box) ||
        !right->bounding_box(t0, t1, right_box)) {
      std::cerr << "No bounding box in bvh_node constructor." << std::endl;
    }

    box = surrounding_box(left_box, right_box);
  }

  bool hit(const ray &r, float t_min, float t_max,
           hit_record &rec) const override {
    if (!box.hit(r, t_min, t_max)) {
      return false;
    }

    hit_record left_rec;
    bool hit_left = left->hit(r, t_min, t_max, left_rec);
    hit_record right_rec;
    bool hit_right = right->hit(r, t_min, t_max, right_rec);
    if (hit_left && hit_right) {
      rec = left_rec.t < right_rec.t ? left_rec : right_rec;
    } else if (hit_left) {
      rec = left_rec;
    } else if (hit_right) {
      rec = right_rec;
    }

    return hit_left || hit_right;
  }

  ~bvh_node() {
    delete left;
    if (left != right) {
      delete right;
    }
  }

  bool bounding_box(float t0 __attribute__((unused)),
                    float t1 __attribute__((unused)),
                    aabb &output_box) const override {
    output_box = box;
    return true;
  }

public:
  hittable *left;
  hittable *right;
  aabb box;
};

#endif // BVH_H_
