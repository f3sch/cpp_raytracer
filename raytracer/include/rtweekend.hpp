#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

template <typename T> inline T random_draw() {
  // Returns a random real in [0,1).
  static std::uniform_real_distribution<T> dist(0.0, 1.0);
  static std::mt19937 gen;
  return dist(gen);
}

template <typename T> inline T random_draw(T min, T max) {
  // Returns a random real in [min,max).
  static std::uniform_real_distribution<T> dist(min, max);
  static std::mt19937 gen;
  return dist(gen);
}

template <> inline int random_draw(int min, int max) {
  // Returns a random real in [0,1).
  static std::uniform_int_distribution<int> dist(0.0, 1.0);
  static std::mt19937 gen;
  return dist(gen);
}

inline double clamp(double x, double min, double max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

// Common Headers

#include "ray.hpp"
#include "vec3.hpp"

struct RGB {
  unsigned int red;
  unsigned int green;
  unsigned int blue;

  inline void print(std::ostream &out) {
    out << red << ' ' << green << ' ' << blue << '\n';
  }

  RGB(int r, int g, int b) : red(r), green(g), blue(b) {}
  RGB() : red(0), green(0), blue(0) {}

  inline void make_color(raytracer::Color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for
    // gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = std::sqrt(scale * r);
    g = std::sqrt(scale * g);
    b = std::sqrt(scale * b);

    red = static_cast<unsigned int>(256 * clamp(r, 0.0, 0.999));
    green = static_cast<unsigned int>(256 * clamp(g, 0.0, 0.999));
    blue = static_cast<unsigned int>(256 * clamp(b, 0.0, 0.999));
  }
};

#endif
