#ifndef _COLOR
#define _COLOR

#include "hittable.hpp"
#include "material.hpp"
#include "rtweekend.hpp"
#include <cmath>
#include <iostream>
#include <vector>

namespace raytracer {

inline void write_color(std::ostream &out, Color pixel_color,
                        int samples_per_pixel) {
  // Write the translated [0,255] value of each color component.
  RGB rgb;
  rgb.make_color(pixel_color, samples_per_pixel);
  rgb.print(out);
}

inline void write_color(std::vector<RGB> &buf, Color pixel_color,
                        int samples_per_pixel) {
  RGB rgb;
  rgb.make_color(pixel_color, samples_per_pixel);
  buf.push_back(rgb);
}

inline void write_color(std::vector<RGB> &buf, int j, int i, int width,
                        Color pixel_color, int samples_per_pixel) {
  RGB rgb;
  rgb.make_color(pixel_color, samples_per_pixel);
  buf.at(j * width + i) = rgb;
}

inline double hit_sphere(const Point &center, double radius, const Ray &r) {
  auto oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;
  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-half_b - sqrt(discriminant)) / a;
  }
}

inline Color mult_col(const Color &c1, const Color &c2) {
  return Color(c1.x() * c2.x(), c1.y() * c2.y(), c1.z() * c2.z());
}

inline Color ray_color(const Ray &r, const hittable &world, int depth) {
  hit_record rec;

  if (depth <= 0)
    return Color(0, 0, 0);

  if (world.hit(r, 0.001, infinity, rec)) {
    Ray scattered;
    Color attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      return mult_col(attenuation, ray_color(scattered, world, depth - 1));
    return Color(0, 0, 0);
  }

  Vector unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

} // namespace raytracer

#endif
