#ifndef _RAY
#define _RAY

#include "vec3.hpp"

namespace raytracer {

class Ray {
public:
  Ray() {}
  Ray(const Point &origin, const Vector &direction, double time = 0.3)
      : orig(origin), dir(direction), tm(time) {}
  Point origin() const { return orig; }
  Vector direction() const { return dir; }
  double time() const { return tm; }

  Point at(double t) const { return orig + t * dir; }

private:
  Point orig;
  Vector dir;
  double tm;
};

} // namespace raytracer

#endif
