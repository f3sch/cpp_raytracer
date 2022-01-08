#include "rtweekend.hpp"

#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"

#include <iostream>

using namespace raytracer;

int main() {

  // Image

  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 25;
  const int max_depth = 25;

  // World
  hittable_list world;

  auto material_ground = make_shared<lambertian>(Color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(Color(0.7, 0.3, 0.3));
  auto material_left = make_shared<dielectric>(1.5);
  auto material_right = make_shared<metal>(Color(0.8, 0.6, 0.2));

  world.add(
      make_shared<sphere>(Point(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(Point(0.0, 0.0, -1.0), 0.5, material_center));
  world.add(make_shared<sphere>(Point(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<sphere>(Point(-1.0, 0.0, -1.0), -0.4, material_left));
  world.add(make_shared<sphere>(Point(1.0, 0.0, -1.0), 0.5, material_right));

  // Camera
  Camera cam(Point(-2, 2, 1), Point(0, 0, -1), Vector(0, 1, 0), 20,
             aspect_ratio);

  // Render

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      Color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        auto u = (i + random_draw<double>()) / (image_width - 1);
        auto v = (j + random_draw<double>()) / (image_height - 1);
        Ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world, max_depth);
      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }

  std::cerr << "\nDone.\n";
}
