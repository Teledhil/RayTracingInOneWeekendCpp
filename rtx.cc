#include <iostream>
#include <thread>

#include "camera.h"
#include "color.h"
#include "dielectric.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "ray.h"
#include "sphere.h"
#include "utility.h"
#include "vec3.h"

color ray_color(const ray &r, const hittable_list &world, int depth) {
  // Limit how much the ray can bounce around.
  if (depth <= 0) {
    return black;
  }

  hit_record rec;
  if (world.hit(r, 0.001, infinity, rec)) {
    ray scattered;
    color attenuation;
    if (rec.m->scatter(r, rec, attenuation, scattered)) {
      return attenuation * ray_color(scattered, world, depth - 1);
    }
    return black;
  }

  // Hits infinity
  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5 * (unit_direction.y() + 1.0);

  // Linear Interpolation (Lerp) from white to blue
  return (1.0 - t) * white + t * blue;
}

hittable_list old_scene() {
  // World objects
  hittable_list world;
  // center sphere
  world.add(
      new sphere(point3(0, 0, -1), 0.5, new lambertian(color(0.1, 0.2, 0.5))));
  // the earth
  world.add(new sphere(point3(0, -100.5, -1), 100,
                       new lambertian(color(0.8, 0.8, 0.0))));
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
  world.add(new sphere(point3(1, 0, -1), -0.45, // bubble
                       new dielectric(dielectric::GLASS_REFRACTION_INDEX)));

  return world;
}

hittable_list random_scene() {
  hittable_list world;

  // The Ground
  material *ground_material = new lambertian(color(0.5, 0.5, 0.5));
  world.add(new sphere(point3(0, -1000.0, 0), 1000, ground_material));

  // Some random mini spheres
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_material = random_float();
      float mini_sphere_radius = 0.2;
      point3 center(a + 0.9 * random_float(), mini_sphere_radius,
                    b + 0.9 * random_float());

      // if not too clse to the center
      if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
        material *sphere_material;
        sphere *mini_sphere;
        if (choose_material < 0.8) {
          // diffuse
          color albedo = random_vec3() * random_vec3();
          sphere_material = new lambertian(albedo);
        } else if (choose_material < 0.95) {
          // metal
          color albedo = random_vec3(0.5, 1);
          float fuzziness = random_float(0, 0.2);
          sphere_material = new metal(albedo, fuzziness);
        } else {
          // dielectric
          sphere_material = new dielectric(dielectric::GLASS_REFRACTION_INDEX);
        }
        mini_sphere = new sphere(center, mini_sphere_radius, sphere_material);
        world.add(mini_sphere);
      }
    }
  }

  // The Big 3
  material *material1 = new dielectric(dielectric::GLASS_REFRACTION_INDEX);
  world.add(new sphere(point3(0, 1, 0), 1.0, material1));
  material *material2 = new lambertian(color(0.4, 0.2, 0.1));
  world.add(new sphere(point3(-4, 1, 0), 1.0, material2));
  material *material3 = new metal(color(0.7, 0.6, 0.5), 0.0);
  world.add(new sphere(point3(4, 1, 0), 1.0, material3));

  return world;
}

void simple_rtx(const camera &cam, int samples_per_pixel, int max_depth,
                const hittable_list &world) {
  // PPM header
  std::cout << "P3" << std::endl;
  std::cout << cam.getWidth() << " " << cam.getHeight() << std::endl;
  std::cout << "255" << std::endl;

  // RTX on!
  for (int j = cam.getHeight() - 1; j >= 0; --j) {
    for (int i = 0; i < cam.getWidth(); ++i) {
      color pixel(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        float u = (i + random_float()) / (cam.getWidth() - 1);
        float v = (j + random_float()) / (cam.getHeight() - 1);

        ray r = cam.get_ray(u, v);
        pixel += ray_color(r, world, max_depth);
      }
      write_color(std::cout, pixel, samples_per_pixel);
    }
  }
}

void thread_work(const camera &cam, int samples_per_pixel, int max_depth,
                 const hittable_list &world,
                 std::vector<std::vector<color>> &canvas) {
  std::vector<std::vector<color>> local_canvas(
      cam.getHeight(), std::vector<color>(cam.getWidth()));
  for (int j = cam.getHeight() - 1; j >= 0; --j) {
    for (int i = 0; i < cam.getWidth(); ++i) {
      color &pixel = local_canvas[j][i];
      pixel = black;
      for (int s = 0; s < samples_per_pixel; ++s) {
        float u = (i + random_float()) / (cam.getWidth() - 1);
        float v = (j + random_float()) / (cam.getHeight() - 1);

        ray r = cam.get_ray(u, v);
        pixel += ray_color(r, world, max_depth);
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
    threads.emplace_back(thread_work, std::cref(cam), samples_per_thread,
                         max_depth, std::cref(world), std::ref(canvas));
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

  for (int j = cam.getHeight() - thread_id - 1; j >= 0; j -= num_threads) {
    for (int i = 0; i < cam.getWidth(); ++i) {
      color pixel = black;
      for (int s = 0; s < samples_per_pixel; ++s) {
        float u = (i + random_float()) / (cam.getWidth() - 1);
        float v = (j + random_float()) / (cam.getHeight() - 1);

        ray r = cam.get_ray(u, v);
        pixel += ray_color(r, world, max_depth);
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

int main() {
  static constexpr int SAMPLES_PER_PIXEL = 96;
  static constexpr int MAX_DEPTH = 16;

  vec3 lookfrom = vec3(13, 2, 3);
  vec3 lookat = vec3(0, 0, 0);
  vec3 vup = vec3(0, 1, 0);
  float vfov = 20;
  float aperture = 0.1;
  // float distance_to_focus = (lookfrom - lookat).length();
  float distance_to_focus = 10.0;

  camera cam(lookfrom, lookat, vup, vfov, aperture, distance_to_focus);

  // World objects
  hittable_list world = random_scene();
  // hittable_list world = old_scene();

  // simple_rtx(cam, SAMPLES_PER_PIXEL, MAX_DEPTH, world);
  // parallelize_by_samples(cam, SAMPLES_PER_PIXEL, MAX_DEPTH, world);
  parallelize_by_lines(cam, SAMPLES_PER_PIXEL, MAX_DEPTH, world);

  return 0;
}
