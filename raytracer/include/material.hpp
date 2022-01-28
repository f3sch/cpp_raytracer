#ifndef _MATERIAL
#define _MATERIAL

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include "hittable.hpp"
#include "perlin.hpp"
#include "rtweekend.hpp"

#include <filesystem>
#include <iostream>

namespace raytracer {

class texture {
public:
  virtual Color value(double u, double v, const Point &p) const = 0;
};

class solid_color : public texture {
public:
  solid_color() {}
  solid_color(Color c) : color_value(c) {}
  solid_color(double r, double g, double b) : color_value(r, g, b) {}

  virtual Color value(double u, double v, const Point &p) const override {
    return color_value;
  }

private:
  Color color_value;
};

class noise_texture : public texture {
public:
  noise_texture() : scale(1) {}
  noise_texture(double sc) : scale(sc) {}
  virtual Color value(double u, double v, const Point &p) const override {
    return Color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
  }

private:
  perlin noise;
  double scale;
};

class checker_texture : public texture {
public:
  checker_texture() {}

  checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
      : odd(_odd), even(_even) {}

  checker_texture(Color c1, Color c2)
      : odd(make_shared<solid_color>(c2)), even(make_shared<solid_color>(c1)) {}

  virtual Color value(double u, double v, const Point &p) const override {
    auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (sines < 0)
      return odd->value(u, v, p);
    else
      return even->value(u, v, p);
  }

public:
  shared_ptr<texture> odd;
  shared_ptr<texture> even;
};

class material {
public:
  virtual bool scatter(const Ray &r_in, const hit_record &rec, Color &att,
                       Ray &scattered) const = 0;
};

class lambertian : public material {
public:
  lambertian(const Color &a) : albedo(make_shared<solid_color>(a)) {}
  lambertian(shared_ptr<texture> a) : albedo(a) {}

  virtual bool scatter(const Ray &r_in, const hit_record &rec,
                       Color &attenuation, Ray &scattered) const override {
    auto scatter_direction = rec.n + random_unit_vector<DataType>();

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
      scatter_direction = rec.n;

    scattered = Ray(rec.p, scatter_direction, r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
  }

public:
  shared_ptr<texture> albedo;
};

class metal : public material {
public:
  metal(const Color &a, double f = 0.0) : albedo(a), fuzz(f < 1 ? f : 1) {}

  virtual bool scatter(const Ray &r_in, const hit_record &rec,
                       Color &attenuation, Ray &scattered) const override {
    Vector reflected = reflect(unit_vector(r_in.direction()), rec.n);
    scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere<DataType>(),
                    r_in.time());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.n) > 0);
  }

public:
  Color albedo;
  double fuzz;
};

class dielectric : public material {
public:
  dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  virtual bool scatter(const Ray &r_in, const hit_record &rec,
                       Color &attenuation, Ray &scattered) const override {
    attenuation = Color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    Vector unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.n), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector direction;

    if (cannot_refract ||
        reflectance(cos_theta, refraction_ratio) > random_draw<double>())
      direction = reflect(unit_direction, rec.n);
    else
      direction = refract(unit_direction, rec.n, refraction_ratio);

    scattered = Ray(rec.p, direction, r_in.time());
    return true;
  }

private:
  double ir; // Index of Refraction

  static double reflectance(double cosine, double ref_idx) {
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

class image_texture : public texture {
public:
  const static int bytes_per_pixel = 3;

  image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

  image_texture(const char *filename) {
    auto components_per_pixel = bytes_per_pixel;

    data = stbi_load(filename, &width, &height, &components_per_pixel,
                     components_per_pixel);

    if (!data) {
      std::cerr << "ERROR: Could not load texture image file '" << filename
                << "'.\n";
      width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
  }

  ~image_texture() { delete data; }

  virtual Color value(double u, double v, const Vector &p) const override {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (data == nullptr)
      return Color(0, 1, 1);

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width)
      i = width - 1;
    if (j >= height)
      j = height - 1;

    const auto color_scale = 1.0 / 255.0;
    auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return Color(color_scale * pixel[0], color_scale * pixel[1],
                 color_scale * pixel[2]);
  }

private:
  unsigned char *data;
  int width, height;
  int bytes_per_scanline;
};

} // namespace raytracer

#endif
