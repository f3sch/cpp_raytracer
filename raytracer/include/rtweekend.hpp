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

#endif
