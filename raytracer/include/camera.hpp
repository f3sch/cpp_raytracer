#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.hpp"

namespace raytracer {

class Camera {
public:
  Camera(Point lookfrom, Point lookat, Vector vup, double vfov,
         double aspect_ratio, double aperture, double focus_dist, double _time0,
         double _time1) {

    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    horizontal = viewport_width * u * focus_dist;
    vertical = viewport_height * v * focus_dist;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - w * focus_dist;
    lens_radius = aperture / 2;

    time0 = _time0;
    time1 = _time1;
  }

  Ray get_ray(double s, double t) const {
    Vector rd = lens_radius * random_in_unit_disk<double>();
    Vector offset = u * rd.x() + v * rd.y();
    // Vector offset = u * rd.x() + v * rd.y();
    return Ray(origin + offset,
               lower_left_corner + s * horizontal + t * vertical - origin -
                   offset,
               random_draw<double>(time0, time1));
  }

private:
  Point origin;
  Point lower_left_corner;
  Vector horizontal;
  Vector vertical;
  Vector u, v, w;
  double lens_radius;
  double time0, time1;
};

} // namespace raytracer
#endif
