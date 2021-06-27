#pragma once

#include "hittable_list.h"
#include "isotropic.h"
#include "material.h"
#include "material_manager.h"
#include "random.h"

namespace rtx {
class World {
  public:
    World() : materials_(), scene_randomness_() {}

    hittable_list final_scene() {

      // Ground boxes
      //
      hittable_list boxes1;
      material *ground_material =
          materials_.new_lambertian(new solid_color(0.48, 0.83, 0.53));
      std::vector<material *> ground{};
      for (int i = 0; i < 6; ++i) {
        ground.emplace_back(ground_material);
      }
      constexpr int BOXES_PER_SIDE = 20;
      for (int i = 0; i < BOXES_PER_SIDE; ++i) {
        for (int j = 0; j < BOXES_PER_SIDE; ++j) {
          float w = 100.0;

          float x0 = -1000.0 + i * w;
          float z0 = -1000.0 + j * w;
          float y0 = 0.0;

          float x1 = x0 + w;
          float y1 = scene_randomness_.random_float(1, 101);
          float z1 = z0 + w;

          // ground.emplace_back(
          //    new lambertian(new solid_color(0.48, 0.83, 0.53)));
          // ground.emplace_back(
          //    new lambertian(new solid_color(0.48, 0.83, 0.53)));
          // ground.emplace_back(
          //    new lambertian(new solid_color(0.48, 0.83, 0.53)));
          // ground.emplace_back(
          //    new lambertian(new solid_color(0.48, 0.83, 0.53)));
          // ground.emplace_back(
          //    new lambertian(new solid_color(0.48, 0.83, 0.53)));
          // ground.emplace_back(
          //    new lambertian(new solid_color(0.48, 0.83, 0.53)));
          boxes1.add(new box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
        }
      }
      boxes1.optimize_bvh(scene_randomness_);

      hittable_list world;
      world.add(boxes1);

      // light
      //
      material *light = materials_.new_diffuse_light(new solid_color(7, 7, 7));
      // material *light = new diffuse_light(new solid_color(7, 7, 7));
      world.add(new xz_rect(123, 423, 554, 147, 412, light));

      // glass ball
      //
      material *glass_material = materials_.new_glass();
      world.add(new sphere(point3(260, 150, 45), 50, glass_material));

      // diffuse metal ball
      //
      material *diffuse_ball_material =
          materials_.new_metal(color(0.8, 0.8, 0.9), 10.0);
      world.add(new sphere(point3(0, 150, 145), 50, diffuse_ball_material));

      // blue caustic ball
      world.add(new sphere(point3(360, 150, 145), 70, glass_material));
      world.add(new constant_medium(
          new sphere(point3(360, 150, 145), 70, glass_material),
          materials_.new_isotropic(new solid_color(0.2, 0.4, 0.9)), 0.2));
      // world.add(new constant_medium(
      //    new sphere(point3(360, 150, 145), 70, glass_material),
      //    new isotropic(new solid_color(0.2, 0.4, 0.9)), 0.2));

      // mist
      isotropic *mist_material =
          materials_.new_isotropic(new solid_color(1, 1, 1));
      world.add(
          new constant_medium(new sphere(point3(0, 0, 0), 5000, glass_material),
                              mist_material, 0.0001));

      // the earth
      // material *earth_map = new lambertian(new
      // image_texture("earthmap.jpg"));
      material *earth_map =
          materials_.new_lambertian(new image_texture("earthmap.jpg"));
      world.add(new sphere(point3(400, 200, 400), 100, earth_map));

      // pseudo jupiter
      material *jupiter_material =
          materials_.new_lambertian(new noise_texture(0.12, scene_randomness_));
      world.add(new sphere(point3(220, 280, 300), 80, jupiter_material));

      // cluster box
      //
      hittable_list cluster_box;
      constexpr int LITTLE_BALLS = 1000;
      material *cluster_material =
          materials_.new_lambertian(new solid_color(0.73, 0.73, 0.73));
      for (int i = 0; i < LITTLE_BALLS; ++i) {
        cluster_box.add(new sphere(scene_randomness_.random_vec3(0, 165), 10,
                                   cluster_material));
      }
      cluster_box.optimize_bvh(scene_randomness_);
      hittable *optimized_cluster_box = cluster_box.eject_bvh();

      optimized_cluster_box = new rotate_y(optimized_cluster_box, 15);
      optimized_cluster_box =
          new translate(optimized_cluster_box, vec3(-100, 270, 395));
      world.add(optimized_cluster_box);

      std::cerr << "Building optimized world... ";
      world.optimize_bvh(scene_randomness_);
      std::cerr << "Done" << std::endl;
      return world;
    }

  private:
    MaterialManager materials_;
    rtx::random scene_randomness_;
};
} // namespace rtx
