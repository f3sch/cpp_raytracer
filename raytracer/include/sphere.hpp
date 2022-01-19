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

  virtual bool bounding_box(double time0, double time1,
                            aabb &output_box) const override;

private:
  Point center;
  double radius;
  shared_ptr<material> mat_ptr;

  inline std::pair<double, double>
  u_v_from_sphere_hit_point(const Point &hit_point) const {
    auto theta = acos(-hit_point.y());
    auto phi = atan2(-hit_point.z(), hit_point.x()) + pi;
    auto u = phi / (2 * pi);
    auto v = theta / pi;
    return {u, v};
  }
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
  // Calculate hit point on sphere
  auto [u, v] = u_v_from_sphere_hit_point(rec.p - center);

  rec.t = root;
  rec.p = r.at(rec.t);
  rec.n = (rec.p - center) / radius;
  rec.u = u;
  rec.v = v;
  Vector outward_n = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_n);
  rec.mat_ptr = mat_ptr;

  return true;
}

inline bool sphere::bounding_box(double time0, double time1,
                                 aabb &output_box) const {
  output_box = aabb{center - Vector{radius, radius, radius},
                    center + Vector{radius, radius, radius}};
  return true;
}

class moving_sphere : public hittable {
public:
  moving_sphere() {}
  moving_sphere(Point cen0, Point cen1, double _time0, double _time1, double r,
                shared_ptr<material> m)
      : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r),
        mat_ptr(m){};

  virtual bool hit(const Ray &r, double t_min, double t_max,
                   hit_record &rec) const override;

  virtual bool bounding_box(double _time0, double _time1,
                            aabb &output_box) const override;

  Point center(double time) const;

public:
  Point center0, center1;
  double time0, time1;
  double radius;
  shared_ptr<material> mat_ptr;
};

inline Point moving_sphere::center(double time) const {
  return center0 + (time - time0) / (time1 - time0) * (center1 - center0);
}

inline bool moving_sphere::hit(const Ray &r, double t_min, double t_max,
                               hit_record &rec) const {
  Vector oc = r.origin() - center(r.time());
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
  auto outward_normal = (rec.p - center(r.time())) / radius;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr;

  return true;
}

inline bool moving_sphere::bounding_box(double _time0, double _time1,
                                        aabb &output_box) const {
  aabb box0(center(_time0) - Vector(radius, radius, radius),
            center(_time0) + Vector(radius, radius, radius));
  aabb box1(center(_time1) - Vector(radius, radius, radius),
            center(_time1) + Vector(radius, radius, radius));
  output_box = surrounding_box(box0, box1);
  return true;
}

} // namespace raytracer

#endif
