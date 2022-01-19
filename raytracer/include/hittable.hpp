#ifndef _HITTABLE
#define _HITTABLE

#include "aabb.hpp"
#include "ray.hpp"
#include "rtweekend.hpp"
#include "vec3.hpp"

namespace raytracer {

class material;

struct hit_record {
  Point p;
  Vector n;
  shared_ptr<material> mat_ptr;
  double t;
  bool front_face;
  double u;
  double v;

  inline void set_face_normal(const Ray &r, const Vector &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    n = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual bool hit(const Ray &r, double t_min, double t_max,
                   hit_record &rec) const = 0;
  virtual bool bounding_box(double time0, double time1,
                            aabb &output_box) const = 0;
};

} // namespace raytracer

#endif
