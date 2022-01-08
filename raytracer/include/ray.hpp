#ifndef _RAY
#define _RAY

#include "vec3.hpp"

namespace raytracer {

class Ray {
public:
  Ray() {}
  Ray(const Point &origin, const Vector &direction)
      : orig(origin), dir(direction) {}
  Point origin() const { return orig; }
  Vector direction() const { return dir; }

  Point at(double t) const { return orig + t * dir; }

private:
  Point orig;
  Vector dir;
};

} // namespace raytracer

#endif
