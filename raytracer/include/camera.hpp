#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.hpp"

namespace raytracer::camera {
using namespace raytracer::geo;
using namespace raytracer::ray;

class Camera {
public:
  Camera() {
    auto aspect_ratio = 16.0 / 9.0;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    origin = Point(0, 0, 0);
    horizontal = Vector(viewport_width, 0.0, 0.0);
    vertical = Vector(0.0, viewport_height, 0.0);
    lower_left_corner =
        origin - horizontal / 2 - vertical / 2 - Vector(0, 0, focal_length);
  }

  Ray get_ray(double u, double v) const {
    return Ray(origin,
               lower_left_corner + u * horizontal + v * vertical - origin);
  }

private:
  Point origin;
  Point lower_left_corner;
  Vector horizontal;
  Vector vertical;
};

} // namespace raytracer::camera
#endif
