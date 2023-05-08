#include <iostream>
#include <thread>
#include <vector>

#include "box.h"
#include "bvh.h"
#include "camera.h"
#include "checker_texture.h"
#include "color.h"
#include "constant_medium.h"
#include "dielectric.h"
#include "diffuse_light.h"
#include "flip_face.h"
#include "hittable_list.h"
#include "image_texture.h"
#include "lambertian.h"
#include "metal.h"
#include "noise_texture.h"
#include "random.h"
#include "ray.h"
#include "rotate_y.h"
#include "solid_color.h"
#include "sphere.h"
#include "translate.h"
#include "utility.h"
#include "vec3.h"
#include "xy_rect.h"
#include "xz_rect.h"
#include "yz_rect.h"

using namespace rtx;

color ray_color(const ray &r, const hittable_list &world, int depth,
                rtx::random &ran) {
  // Limit how much the ray can bounce around.
  if (depth <= 0) {
    return black;
  }

  hit_record rec;
  if (!world.hit(r, 0.001, infinity, rec, ran)) {
    // or color background.
    return black;
  }

  color emitted = rec.m->emitted(rec.u, rec.v, rec.p);
  ray scattered;
  color attenuation;
  if (!rec.m->scatter(r, rec, attenuation, scattered, ran)) {
    return emitted;
  }
  return emitted + attenuation * ray_color(scattered, world, depth - 1, ran);

  //// Hits infinity
  // vec3 unit_direction = unit_vector(r.direction());
  // float t = 0.5 * (unit_direction.y() + 1.0);

  //// Linear Interpolation (Lerp) from white to blue
  // return (1.0 - t) * white + t * blue;
}

hittable_list old_scene(rtx::random &ran __attribute__((unused))) {
  // World objects
  hittable_list world;
  // center sphere
  world.add(new sphere(point3(0, 0, -1), 0.5,
                       new lambertian(new solid_color(0.1, 0.2, 0.5))));
  // the earth
  world.add(new sphere(point3(0, -100.5, -1), 100,
                       new lambertian(new solid_color(0.8, 0.8, 0.0))));
  // mettalic spheres
  // world.add(
  //    new sphere(point3(1, 0, -1), 0.5, new metal(color(0.8, 0.6, 0.2),
  //    0.0)));
  world.add(
      new sphere(point3(-1, 0, -1), 0.5, new metal(color(0.8, 0.8, 0.8), 0.0)));
  // dielectric spheres
  // world.add(new sphere(point3(1, 0, -1), 0.5,
  //                     new dielectric(dielectric::GLASS_REFRACTION_INDEX)));
  world.add(new sphere(point3(1, 0, -1), 0.5, // bubble
                       new dielectric(dielectric::GLASS_REFRACTION_INDEX)));
  // world.add(new sphere(point3(1, 0, -1), -0.45, // bubble
  //                     new dielectric(dielectric::GLASS_REFRACTION_INDEX)));

  return world;
}

hittable_list random_scene(rtx::random &ran) {
  hittable_list world;


  // The Ground
  rtx::texture *ground_texture = new checker_texture(
      new solid_color(0.2, 0.3, 0.1), new solid_color(0.9, 0.9, 0.9));
  material *ground_material = new lambertian(ground_texture);
  world.add(new sphere(point3(0, -1000.0, 0), 1000, ground_material));

  // Some random mini spheres
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_material = ran.random_float();
      float mini_sphere_radius = 0.2;
      point3 center(a + 0.9 * ran.random_float(), mini_sphere_radius,
                    b + 0.9 * ran.random_float());

      // if not too close to the center
      if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
        material *sphere_material;
        sphere *mini_sphere;
        point3 light_offset(0, 0, 0);
        if (choose_material < 0.7) {
          // diffuse
          const color albedo = ran.random_vec3() * ran.random_vec3();
          sphere_material = new lambertian(new solid_color(albedo));
        } else if (choose_material < 0.8) {
          sphere_material = new diffuse_light(new solid_color(white));
          light_offset = point3(0, 2.5, 0);
        } else if (choose_material < 0.95) {
          // metal
          color albedo = ran.random_vec3(0.5, 1);
          float fuzziness = ran.random_float(0, 0.2);
          sphere_material = new metal(albedo, fuzziness);
        } else {
          // dielectric
          sphere_material = new dielectric(dielectric::GLASS_REFRACTION_INDEX);
        }
        mini_sphere = new sphere(center + light_offset, mini_sphere_radius,
                                 sphere_material);
        world.add(mini_sphere);
      }
    }
  }

  // The Big 3
  material *material1 = new dielectric(dielectric::GLASS_REFRACTION_INDEX);
  world.add(new sphere(point3(0, 1, 0), 1.0, material1));
  material *material2 = new lambertian(new image_texture("earthmap.jpg"));
  world.add(new sphere(point3(-4, 1, 0), 1.0, material2));
  material *material3 = new metal(color(0.7, 0.6, 0.5), 0.0);
  world.add(new sphere(point3(4, 1, 0), 1.0, material3));

  return world;
}

hittable_list two_perlin_spheres(rtx::random &ran) {
  hittable_list world;

  // texture *perlin_texture = new noise_texture();

  world.add(new sphere(point3(0, -1000, 0), 1000,
                       new lambertian(new noise_texture(2.0, ran))));
  world.add(new sphere(point3(0, 2, 0), 2,
                       new lambertian(new noise_texture(10.0, ran))));

  material *difflight_rectangle =
      new diffuse_light(new solid_color(color(4, 4, 4)));
  world.add(new xy_rect(1, 3, 1, 3, -2, difflight_rectangle));
  material *difflight_rectangle_2 =
      new diffuse_light(new solid_color(color(4, 4, 4)));
  world.add(new xy_rect(1, 3, 1, 3, 2, difflight_rectangle_2));

  material *difflight_rectangle2 =
      new diffuse_light(new solid_color(color(4, 4, 4)));
  world.add(new xz_rect(1, 3, 2, 1, 3, difflight_rectangle2));

  material *difflight_rectangle3 =
      new diffuse_light(new solid_color(color(4, 4, 4)));
  world.add(new yz_rect(2, 1, 3, 1, 3, difflight_rectangle3));
  material *difflight_rectangle32 =
      new diffuse_light(new solid_color(color(4, 4, 4)));
  world.add(new yz_rect(-2, 1, 3, 1, 3, difflight_rectangle32));

  material *difflight_sphere =
      new diffuse_light(new solid_color(color(4, 4, 4)));
  world.add(new sphere(point3(0, 7, 0), 2, difflight_sphere));

  return world;
}

hittable_list the_earth(rtx::random &ran __attribute__((unused))) {
  hittable_list world;
  world.add(new sphere(point3(0, 2, 0), 2,
                       new lambertian(new image_texture("earthmap.jpg"))));

  return world;
}

hittable_list cornell_box(rtx::random &ran __attribute__((unused))) {
  hittable_list world;

  // back
  world.add(new xy_rect(0, 555, 0, 555, 555,
                        new lambertian(new solid_color(color(.73, .73, .73)))));

  // bottom
  world.add(new xz_rect(0, 555, 0, 0, 555,
                        new lambertian(new solid_color(color(.73, .73, .73)))));

  // top
  world.add(new flip_face(
      new xz_rect(0, 555, 555, 0, 555,
                  new lambertian(new solid_color(color(.73, .73, .73))))));

  // left
  world.add(new flip_face(
      new yz_rect(0, 0, 555, 0, 555,
                  new lambertian(new solid_color(color(.12, .45, .15))))));

  // right
  world.add(new yz_rect(555, 0, 555, 0, 555,
                        new lambertian(new solid_color(color(.65, .05, .05)))));

  // light
  world.add(new xz_rect(213, 343, 554, 227, 332,
                        new diffuse_light(new solid_color(color(15, 15, 15)))));

  std::vector<material *> m1;
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  hittable *box1 = new box(point3(0, 0, 0), point3(165, 330, 165), m1);
  box1 = new rotate_y(box1, 15);
  box1 = new translate(box1, vec3(265, 0, 295));

  world.add(box1);

  std::vector<material *> m2;
  m2.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m2.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m2.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m2.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m2.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m2.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  hittable *box2 = new box(point3(0, 0, 0), point3(165, 165, 165), m2);
  box2 = new rotate_y(box2, -18);
  box2 = new translate(box2, vec3(130, 0, 65));
  world.add(box2);

  return world;
}

hittable_list cornell_box_with_fog(rtx::random &ran __attribute__((unused))) {
  hittable_list world;

  // back
  world.add(new xy_rect(0, 555, 0, 555, 555,
                        new lambertian(new solid_color(color(.73, .73, .73)))));

  // bottom
  world.add(new xz_rect(0, 555, 0, 0, 555,
                        new lambertian(new solid_color(color(.73, .73, .73)))));

  // top
  world.add(new flip_face(
      new xz_rect(0, 555, 555, 0, 555,
                  new lambertian(new solid_color(color(.73, .73, .73))))));

  // left
  world.add(new flip_face(
      new yz_rect(0, 0, 555, 0, 555,
                  new lambertian(new solid_color(color(.12, .45, .15))))));

  // right
  world.add(new yz_rect(555, 0, 555, 0, 555,
                        new lambertian(new solid_color(color(.65, .05, .05)))));

  // light
  world.add(new xz_rect(213, 343, 554, 227, 332,
                        new diffuse_light(new solid_color(color(15, 15, 15)))));

  std::vector<material *> m1;
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  m1.emplace_back(new lambertian(new solid_color(color(.73, .73, .73))));
  hittable *box1 = new box(point3(0, 0, 0), point3(165, 330, 165), m1);
  box1 = new rotate_y(box1, 15);
  box1 = new translate(box1, vec3(265, 0, 295));

  world.add(box1);

  world.add(new constant_medium(
      new sphere(point3(200, 200, 200), 100,
                 new lambertian(new solid_color(0.8, 0.8, 0.8))),
      new solid_color(1, 1, 1), 0.01));
  return world;
}

hittable_list final_scene(rtx::random &ran) {

  // Ground boxes
  //
  hittable_list boxes1;
  constexpr int BOXES_PER_SIDE = 20;
  for (int i = 0; i < BOXES_PER_SIDE; ++i) {
    for (int j = 0; j < BOXES_PER_SIDE; ++j) {
      float w = 100.0;

      float x0 = -1000.0 + i * w;
      float z0 = -1000.0 + j * w;
      float y0 = 0.0;

      float x1 = x0 + w;
      float y1 = ran.random_float(1, 101);
      float z1 = z0 + w;

      std::vector<material *> ground;
      ground.emplace_back(new lambertian(new solid_color(0.48, 0.83, 0.53)));
      ground.emplace_back(new lambertian(new solid_color(0.48, 0.83, 0.53)));
      ground.emplace_back(new lambertian(new solid_color(0.48, 0.83, 0.53)));
      ground.emplace_back(new lambertian(new solid_color(0.48, 0.83, 0.53)));
      ground.emplace_back(new lambertian(new solid_color(0.48, 0.83, 0.53)));
      ground.emplace_back(new lambertian(new solid_color(0.48, 0.83, 0.53)));
      boxes1.add(new box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
    }
  }
  boxes1.optimize_bvh(ran);

  hittable_list world;
  world.add(boxes1);

  // water
  //
  // std::vector<material *> water_texture;
  // float water_index = 1.333;
  // water_texture.emplace_back(new dielectric(water_index));
  // water_texture.emplace_back(new dielectric(water_index));
  // water_texture.emplace_back(new dielectric(water_index));
  // water_texture.emplace_back(new dielectric(water_index));
  // water_texture.emplace_back(new dielectric(water_index));
  // water_texture.emplace_back(new dielectric(water_index));
  // world.add(new box(point3(-1000.0, 0.0, -1000.0),
  //                  point3(1000.0, 100.0, 1000.0), water_texture));

  // light
  //
  material *light = new diffuse_light(new solid_color(7, 7, 7));
  world.add(new xz_rect(123, 423, 554, 147, 412, light));

  // glass ball
  //
  world.add(new sphere(point3(260, 150, 45), 50, new dielectric(1.5)));

  // diffuse metal ball
  //
  world.add(new sphere(point3(0, 150, 145), 50,
                       new metal(color(0.8, 0.8, 0.9), 10.0)));

  // blue caustic ball
  world.add(new sphere(point3(360, 150, 145), 70, new dielectric(1.5)));
  world.add(new constant_medium(
      new sphere(point3(360, 150, 145), 70, new dielectric(1.5)),
      new solid_color(0.2, 0.4, 0.9), 0.2));

  // mist
  world.add(new constant_medium(
      new sphere(point3(0, 0, 0), 5000, new dielectric(1.5)),
      new solid_color(1, 1, 1), 0.0001));

  // the earth
  material *earth_map = new lambertian(new image_texture("earthmap.jpg"));
  world.add(new sphere(point3(400, 200, 400), 100, earth_map));

  // pseudo jupiter
  world.add(new sphere(point3(220, 280, 300), 80,
                       new lambertian(new noise_texture(0.12, ran))));

  // cluster box
  //
  hittable_list cluster_box;
  constexpr int LITTLE_BALLS = 1000;
  for (int i = 0; i < LITTLE_BALLS; ++i) {
    cluster_box.add(
        new sphere(ran.random_vec3(0, 165), 10,
                   new lambertian(new solid_color(0.73, 0.73, 0.73))));
  }
  cluster_box.optimize_bvh(ran);
  hittable *optimized_cluster_box = cluster_box.eject_bvh();

  optimized_cluster_box = new rotate_y(optimized_cluster_box, 15);
  optimized_cluster_box =
      new translate(optimized_cluster_box, vec3(-100, 270, 395));
  world.add(optimized_cluster_box);

  return world;
}

void simple_rtx(const camera &cam, int samples_per_pixel, int max_depth,
                const hittable_list &world) {

  rtx::random ran;

  // PPM header
  std::cout << "P3" << std::endl;
  std::cout << cam.getWidth() << " " << cam.getHeight() << std::endl;
  std::cout << "255" << std::endl;

  // RTX on!
  for (int j = cam.getHeight() - 1; j >= 0; --j) {
    for (int i = 0; i < cam.getWidth(); ++i) {
      color pixel(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        float u = (i + ran.random_float()) / (cam.getWidth() - 1);
        float v = (j + ran.random_float()) / (cam.getHeight() - 1);

        ray r = cam.get_ray(u, v, ran);
        pixel += ray_color(r, world, max_depth, ran);
      }
      write_color(std::cout, pixel, samples_per_pixel);
    }
  }
}

void thread_work_by_sample(const camera &cam, int samples_per_pixel,
                           int max_depth, const hittable_list &world,
                           std::vector<std::vector<color>> &canvas) {
  rtx::random ran;
  std::vector<std::vector<color>> local_canvas(
      cam.getHeight(), std::vector<color>(cam.getWidth()));
  for (int j = cam.getHeight() - 1; j >= 0; --j) {
    for (int i = 0; i < cam.getWidth(); ++i) {
      color &pixel = local_canvas[j][i];
      pixel = black;
      for (int s = 0; s < samples_per_pixel; ++s) {
        float u = (i + ran.random_float()) / (cam.getWidth() - 1);
        float v = (j + ran.random_float()) / (cam.getHeight() - 1);

        ray r = cam.get_ray(u, v, ran);
        pixel += ray_color(r, world, max_depth, ran);
      }
    }
  }

  // Sync canvas
  canvas = std::move(local_canvas);
}

void parallelize_by_samples(const camera &cam, int samples_per_pixel,
                            int max_depth, const hittable_list &world) {
  const unsigned num_threads = std::thread::hardware_concurrency();
  std::cerr << "multithreaded mode: " << num_threads << " threads."
            << std::endl;

  int samples_per_thread = samples_per_pixel / num_threads;
  std::vector<std::thread> threads;
  std::vector<std::vector<std::vector<color>>> multicanvas(num_threads);

  for (unsigned thread_id = 0; thread_id < num_threads; ++thread_id) {
    std::vector<std::vector<color>> &canvas = multicanvas[thread_id];
    threads.emplace_back(thread_work_by_sample, std::cref(cam),
                         samples_per_thread, max_depth, std::cref(world),
                         std::ref(canvas));
  }

  for (auto &t : threads) {
    t.join();
  }
  std::cerr << "All threads completed their tasks." << std::endl;

  std::cerr << "Saving image... ";
  // PPM header
  std::cout << "P3" << std::endl;
  std::cout << cam.getWidth() << " " << cam.getHeight() << std::endl;
  std::cout << "255" << std::endl;

  for (int j = cam.getHeight() - 1; j >= 0; --j) {
    for (int i = 0; i < cam.getWidth(); ++i) {
      color multi_pixel = black;
      for (unsigned thread_id = 0; thread_id < num_threads; ++thread_id) {
        multi_pixel += multicanvas[thread_id][j][i];
      }
      write_color(std::cout, multi_pixel, samples_per_thread * num_threads);
    }
  }
  std::cerr << "Done" << std::endl;
}

void thread_work_by_line(const camera &cam, int samples_per_pixel,
                         int max_depth, const hittable_list &world,
                         std::vector<color> &canvas, unsigned num_threads,
                         int thread_id) {

  rtx::random ran(thread_id);

  for (int j = cam.getHeight() - thread_id - 1; j >= 0; j -= num_threads) {
    for (int i = 0; i < cam.getWidth(); ++i) {
      color pixel = black;
      for (int s = 0; s < samples_per_pixel; ++s) {
        float u = (i + ran.random_float()) / (cam.getWidth() - 1);
        float v = (j + ran.random_float()) / (cam.getHeight() - 1);

        ray r = cam.get_ray(u, v, ran);
        pixel += ray_color(r, world, max_depth, ran);
      }
      canvas[j * cam.getWidth() + i] = pixel;
    }
  }
}

void parallelize_by_lines(const camera &cam, int samples_per_pixel,
                          int max_depth, const hittable_list &world) {
  const unsigned num_threads = std::thread::hardware_concurrency();
  std::cerr << "multithreaded mode: " << num_threads << " threads."
            << std::endl;
  std::vector<color> canvas(cam.getWidth() * cam.getHeight());

  std::vector<std::thread> threads;
  for (unsigned thread_id = 0; thread_id < num_threads; ++thread_id) {
    threads.emplace_back(thread_work_by_line, std::cref(cam), samples_per_pixel,
                         max_depth, std::cref(world), std::ref(canvas),
                         num_threads, thread_id);
  }

  for (auto &t : threads) {
    t.join();
  }
  std::cerr << "All threads completed their tasks." << std::endl;

  std::cerr << "Saving image... ";
  // PPM header
  std::cout << "P3" << std::endl;
  std::cout << cam.getWidth() << " " << cam.getHeight() << std::endl;
  std::cout << "255" << std::endl;

  for (int j = cam.getHeight() - 1; j >= 0; --j) {
    for (int i = 0; i < cam.getWidth(); ++i) {
      write_color(std::cout, canvas[j * cam.getWidth() + i], samples_per_pixel);
    }
  }
  std::cerr << "Done" << std::endl;
}

camera random_scene_camera() {
  point3 lookfrom = vec3(13, 2, 3);
  point3 lookat = vec3(0, 0, 0);
  vec3 vup = vec3(0, 1, 0);
  float vfov = 20;
  float aperture = 0.01;
  // float distance_to_focus = (lookfrom - lookat).length();
  float distance_to_focus = 10.0;
  camera cam(lookfrom, lookat, vup, vfov, aperture, distance_to_focus);
  return cam;
}

camera two_perlin_spheres_camera() {
  point3 lookfrom(13, 2, 3);
  point3 lookat(0, 0, 0);
  vec3 vup(0, 1, 0);
  float vfov = 20;
  float distance_to_focus = 10.0;
  float aperture = 0.1;
  camera cam(lookfrom, lookat, vup, vfov, aperture, distance_to_focus);
  return cam;
}

camera cornell_box_camera() {
  point3 lookfrom(278, 278, -800);
  point3 lookat(278, 278, 0);
  vec3 vup(0, 1, 0);
  float vfov = 40.0;
  float distance_to_focus = 10.0;
  float aperture = 0.0;

  camera cam(lookfrom, lookat, vup, vfov, aperture, distance_to_focus);

  return cam;
}

camera final_scene_camera() {
  point3 lookfrom(478, 278, -600);
  point3 lookat(278, 278, 0);
  vec3 vup(0, 1, 0);
  float vfov = 40.0;
  float distance_to_focus = 10.0;
  float aperture = 0.0;

  camera cam(lookfrom, lookat, vup, vfov, aperture, distance_to_focus);

  return cam;
}

int main() {
  static constexpr int SAMPLES_PER_PIXEL = 500;
  static constexpr int MAX_DEPTH = 20;

  // camera cam = random_scene_camera();
  // camera cam = two_perlin_spheres_camera();
  // camera cam(lookfrom, lookat, vup, vfov, aperture, distance_to_focus);
  // camera cam = cornell_box_camera();
  camera cam = final_scene_camera();

  rtx::random scene_randomness;

  // World objects
  //
  // hittable_list world = random_scene(scene_randomness);
  // hittable_list world = the_earth(scene_randomness);
  // hittable_list world = two_perlin_spheres(scene_randomness);
  // hittable_list world = cornell_box(scene_randomness);
  // hittable_list world = cornell_box_with_fog(scene_randomness);
  // hittable_list world = old_scene(scene_randomness);
  hittable_list world = final_scene(scene_randomness);
  std::cerr << "Building optimized world... ";
  world.optimize_bvh(scene_randomness);
  std::cerr << "Done" << std::endl;

  // simple_rtx(cam, SAMPLES_PER_PIXEL, MAX_DEPTH, world);
  // parallelize_by_samples(cam, SAMPLES_PER_PIXEL, MAX_DEPTH, world);
  parallelize_by_lines(cam, SAMPLES_PER_PIXEL, MAX_DEPTH, world);

  return 0;
}
