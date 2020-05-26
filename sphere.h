#ifndef SPHERE_H_
#define SPHERE_H_

#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

class sphere : public hittable {
public:
  sphere(point3 c, float r, material *m)
      : center_(c), radius_(r), material_(m) {}
  ~sphere() {
    delete material_;
  }

  bool hit(const ray &r, float t_min, float t_max,
           hit_record &rec) const override {
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

  bool bounding_box(float t0 __attribute__((unused)),
                    float t1 __attribute__((unused)),
                    aabb &output_box) const override {
    output_box = aabb(center_ - vec3(radius_, radius_, radius_),
                      center_ + vec3(radius_, radius_, radius_));
    return true;
  }

  const point3 &center() const { return center_; }
  float radius() const { return radius_; }

private:
  point3 center_;
  float radius_;
  material *material_;
};

#endif // SPHERE_H_
