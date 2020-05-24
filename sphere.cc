#include "sphere.h"

#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

// sphere::sphere() : center_(0, 0, 0), radius_(0.0), material_(nullptr) {}
sphere::sphere(point3 c, float r, material *m)
    : center_(c), radius_(r), material_(m) {}

sphere::~sphere() { delete material_; }

bool sphere::hit(const ray &r, float t_min, float t_max,
                 hit_record &rec) const {
  // Solve the quadratic function.

  vec3 oc = r.origin() - center_;

  float a = r.direction().length_squared();
  float half_b = dot(oc, r.direction());
  float c = oc.length_squared() - radius_ * radius_;

  // discriminant > 0  --> 2 hits, entry and exit.
  // discriminant = 0  --> no hit, ray is tangential to sphere.
  // discriminant < 0  --> no hit, ray misses sphere.
  float discriminant = half_b * half_b - a * c;

  if (discriminant > 0) {

    float root = sqrt(discriminant);
    float hit_ray_t = (-half_b - root) / a; // distance travelled by ray until
                                             // it hits the sphere.
    if (hit_ray_t < t_max && hit_ray_t > t_min) {

      // Hit! Record it.
      rec.t = hit_ray_t;
      rec.p = r.at(hit_ray_t);
      vec3 outward_normal = (rec.p - center_) / radius_;
      rec.set_face_normal(r, outward_normal);
      rec.m = material_;
      return true;
    }
    hit_ray_t = (-half_b + root) / a;
    if (hit_ray_t < t_max && hit_ray_t > t_min) {
      // Hit! Record it.
      rec.t = hit_ray_t;
      rec.p = r.at(hit_ray_t);
      vec3 outward_normal = (rec.p - center_) / radius_;
      rec.set_face_normal(r, outward_normal);
      rec.m = material_;
      return true;
    }
  }

  return false;
}
