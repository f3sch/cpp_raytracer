#ifndef _COLOR
#define _COLOR

#include "hittable.hpp"
#include "rtweekend.hpp"
#include <cmath>
#include <iostream>

namespace raytracer::color {
using namespace raytracer::geo;
using namespace raytracer::ray;

inline void write_color(std::ostream &out, Color pixel_color,
                        int samples_per_pixel) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Divide the color by the number of samples and gamma-correct for gamma=2.0.
  auto scale = 1.0 / samples_per_pixel;
  r = std::sqrt(scale * r);
  g = std::sqrt(scale * g);
  b = std::sqrt(scale * b);

  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
      << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

inline double hit_sphere(const Point &center, double radius, const Ray &r) {
  Vec3 oc = r.origin() - center;
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

inline Color ray_color(const Ray &r, const hittable &world, int depth) {
  hit_record rec;

  if (depth <= 0)
    return Color(0, 0, 0);

  if (world.hit(r, 0.001, infinity, rec)) {
    Point target = rec.p + rec.n + random_unit_vector<DataType>();
    return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
  }

  Vector unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

} // namespace raytracer::color

#endif
