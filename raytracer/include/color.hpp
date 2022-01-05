#ifndef _COLOR
#define _COLOR

#include "ray.hpp"
#include "vec3.hpp"
#include <iostream>

namespace raytracer::color {
using namespace raytracer::geo;
using namespace raytracer::ray;

inline void write_color(std::ostream &out, Color pixel_color) {
  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(255.999 * pixel_color.x()) << ' '
      << static_cast<int>(255.999 * pixel_color.y()) << ' '
      << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

inline bool hit_sphere(const Point &center, double radius, const Ray &r) {
  Vec3 oc = r.origin() - center;
  auto a = dot(r.direction(), r.direction());
  auto b = 2.0 * dot(oc, r.direction());
  auto c = dot(oc, oc) - radius * radius;
  auto discriminant = b * b - 4 * a * c;
  return (discriminant > 0);
}

inline Color ray_color(const Ray &r) {
  if (hit_sphere(Point(0, 0, -1), 0.5, r))
    return Color(1, 0, 0);
  Vector unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

} // namespace raytracer::color

#endif
