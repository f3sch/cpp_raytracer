#ifndef _MATERIAL
#define _MATERIAL

#include "hittable.hpp"
#include "rtweekend.hpp"

namespace raytracer {

class material {
public:
  virtual bool scatter(const Ray &r_in, const hit_record &rec, Color &att,
                       Ray &scattered) const = 0;
};

class lambertian : public material {
public:
  lambertian(const Color &a) : albedo(a) {}

  virtual bool scatter(const Ray &r_in, const hit_record &rec,
                       Color &attenuation, Ray &scattered) const override {
    auto scatter_direction = rec.n + random_unit_vector<DataType>();

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
      scatter_direction = rec.n;

    scattered = Ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

public:
  Color albedo;
};

class metal : public material {
public:
  metal(const Color &a, double f = 0.0) : albedo(a), fuzz(f < 1 ? f : 1) {}

  virtual bool scatter(const Ray &r_in, const hit_record &rec,
                       Color &attenuation, Ray &scattered) const override {
    Vector reflected = reflect(unit_vector(r_in.direction()), rec.n);
    scattered =
        Ray(rec.p, reflected + fuzz * random_in_unit_sphere<DataType>());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.n) > 0);
  }

public:
  Color albedo;
  double fuzz;
};

} // namespace raytracer

#endif
