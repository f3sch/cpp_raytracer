#ifndef _HITTABLE
#define _HITTABLE

#include "ray.hpp"
#include "vec3.hpp"

namespace raytracer::geo {
using namespace ray;

struct hit_record {
  Point p;
  Vector n;
  double t;
  bool front_face;

  inline void set_face_normal(const Ray &r, const Vector &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    n = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual bool hit(const Ray &r, double t_min, double t_max,
                   hit_record &rec) const = 0;
};

} // namespace raytracer::geo

#endif