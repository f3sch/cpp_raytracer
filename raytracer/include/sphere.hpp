#ifndef _SPHERE
#define _SPHERE

#include "hittable.hpp"
#include "vec3.hpp"

namespace raytracer {
class sphere : public hittable {
public:
  sphere() {}
  sphere(Point cen, double r, shared_ptr<material> m)
      : center(cen), radius(r), mat_ptr(m) {}

  virtual inline bool hit(const Ray &r, double t_min, double t_max,
                          hit_record &rec) const override;

private:
  Point center;
  double radius;
  shared_ptr<material> mat_ptr;
};

inline bool sphere::hit(const Ray &r, double t_min, double t_max,
                        hit_record &rec) const {
  Vector oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;

  auto discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return false;
  auto sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  auto root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  rec.n = (rec.p - center) / radius;
  Vector outward_n = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_n);
  rec.mat_ptr = mat_ptr;

  return true;
}

} // namespace raytracer

#endif
