#include "rtweekend.hpp"

#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"

#include <iostream>

using namespace raytracer::geo;
using namespace raytracer::color;
using namespace raytracer::ray;
using namespace raytracer::camera;

int main() {

  // Image

  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 300;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 50;
  const int max_depth = 25;

  // World
  hittable_list world;
  world.add(make_shared<sphere>(Point(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(Point(0, -100.5, -1), 100));

  // Camera
  Camera cam;

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
