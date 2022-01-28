#include "rtweekend.hpp"

#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

#include <atomic>
#include <iostream>
#include <omp.h>
#include <vector>

using namespace raytracer;

hittable_list random_scene() {
  hittable_list world;

  auto checker =
      make_shared<checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
  world.add(make_shared<sphere>(Point(0, -1000, 0), 1000,
                                make_shared<lambertian>(checker)));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_draw<double>();
      Point center(a + 0.9 * random_draw<double>(), 0.2,
                   b + 0.9 * random_draw<double>());

      if ((center - Point(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = mult_col(Color::random(), Color::random());
          sphere_material = make_shared<lambertian>(albedo);
          auto center2 = center + Vector(0, random_draw<double>(0, .5), 0);
          world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2,
                                               sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Color::random(0.5, 1);
          auto fuzz = random_draw<double>(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(Point(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(Color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(Point(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(Point(4, 1, 0), 1.0, material3));

  return world;
}

hittable_list two_spheres() {
  hittable_list objects;

  auto checker =
      make_shared<checker_texture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

  objects.add(make_shared<sphere>(Point(0, -10, 0), 10,
                                  make_shared<lambertian>(checker)));

  auto pertext = make_shared<noise_texture>();
  objects.add(make_shared<sphere>(Point(0, 10, 0), 10,
                                  make_shared<lambertian>(pertext)));

  auto sphere_material = make_shared<dielectric>(1.5);
  objects.add(make_shared<sphere>(Point(3, 0, 3), 1, sphere_material));
  // metal
  // auto albedo = Color::random(0.5, 1);
  // auto fuzz = random_draw<double>(0, 0.5);
  // auto sphere_material_m = make_shared<metal>(albedo, fuzz);
  // objects.add(make_shared<sphere>(Point(3, 0, -3), 1, sphere_material_m));

  return objects;
}

hittable_list two_perlin_spheres() {
  hittable_list objects;

  auto pertext = make_shared<noise_texture>(4);
  objects.add(make_shared<sphere>(Point(0, -1000, 0), 1000,
                                  make_shared<lambertian>(pertext)));
  objects.add(
      make_shared<sphere>(Point(0, 2, 0), 2, make_shared<lambertian>(pertext)));

  return objects;
}

hittable_list earth() {
  auto earth_texture = make_shared<image_texture>("../../data/earth.jpg");
  auto earth_surface = make_shared<lambertian>(earth_texture);
  auto globe = make_shared<sphere>(Point(0, 0, 0), 2, earth_surface);

  return hittable_list(globe);
}

int main() {

  // Image

  constexpr auto aspect_ratio = 16.0 / 9.0;
  constexpr int image_width = 400;
  constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 50;
  const int max_depth = 25;

  // World
  // auto world = random_scene();
  // hittable_list world;
  // auto ground_material = make_shared<lambertian>(Color(0.5, 0.5, 0.5));
  // world.add(make_shared<sphere>(Point(0, -1000, 0), 1000, ground_material));
  // auto material1 = make_shared<dielectric>(0.4);
  // world.add(make_shared<sphere>(Point(0, 1, -2), 1.0, material1));
  // auto material3 = make_shared<metal>(Color(0.7, 0.6, 0.5), 0.3);
  // world.add(make_shared<sphere>(Point(2, 1.2, 0), 1.2, material3));
  // auto material4 = make_shared<texture>("./data/earth.jpg", 0.0);
  // world.add(make_shared<sphere>(Point(0, 1.4, -3), 1.4, material4));
  // auto world = random_scene();
  auto world = earth();

  // Camera
  Point lookfrom(13, 2, 3);
  Point lookat(0, 0, 0);
  Vector vup(0, 1, 0);
  auto dist_to_focus = (lookfrom - lookat).length();
  auto aperture = 0.1;

  Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus,
             0.0, 1.0);

  // Buffer
  std::vector<RGB> buf(image_height * image_width);

  // Render

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  // update
  std::atomic<unsigned int> count = 0;
  unsigned int percentage = 0;

#pragma omp parallel for
  for (int j = image_height - 1; j >= 0; --j) {
    count++;
    if (count > static_cast<unsigned int>((image_height * 0.1))) {
      count = 0;
      percentage += 10;
      std::cerr << "\rCompleted: " << percentage << "%" << std::flush;
    }
    for (int i = 0; i < image_width; ++i) {
      Color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        auto u = (i + random_draw<double>()) / (image_width - 1);
        auto v = (j + random_draw<double>()) / (image_height - 1);
        Ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world, max_depth);
      }
      write_color(buf, j, i, image_width, pixel_color, samples_per_pixel);
      // write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }

  std::reverse(buf.begin(), buf.end());
  for (auto rgb : buf) {
    rgb.print(std::cout);
  }
  std::cerr << "\nDone.\n";
}
