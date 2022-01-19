#ifndef _CUBE
#define _CUBE

#include "hittable.hpp"
#include "vec3.hpp"

namespace raytracer {

struct Plane {
  Point P;
  Vector N;

  Plane(Point center, double side) {}
};

class cube : public hittable {
public:
  cube() {}
  cube(Point cen, double side, shared_ptr<material> m)
      : center(cen), side_len(side), mat_ptr(m) {}

  virtual inline bool hit(const Ray &r, double t_min, double t_max,
                          hit_record &rec) const override;

private:
  Point center;
  double side_len;
  std::vector<Plane> planes;
  shared_ptr<material> mat_ptr;
};

inline bool cube::hit(const Ray &r, double t_min, double t_max,
                      hit_record &rec) const {

  return true;
}

} // namespace raytracer

#endif
