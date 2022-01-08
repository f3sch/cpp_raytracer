#ifndef _VEC3
#define _VEC3

#include <assert.h>
#include <cmath>
#include <ostream>

namespace raytracer {
using std::sqrt;

template <typename T> class Vec3 {
public:
  Vec3() : e{0, 0, 0} {}
  Vec3(T e0, T e1, T e2) : e{e0, e1, e2} {}

  T x() const { return e[0]; }
  T y() const { return e[1]; }
  T z() const { return e[2]; }

  Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
  T operator[](int i) const { return e[i]; }
  T &operator[](int i) { return e[i]; }

  Vec3 operator+(const Vec3 &v) const {
    return Vec3(e[0] + v.x(), e[1] + v.y(), e[2] + v.z());
  }
  Vec3 &operator+=(const Vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }
  Vec3 operator-(const Vec3 &v) const {
    return Vec3(e[0] - v.x(), e[1] - v.y(), e[2] - v.z());
  }
  Vec3 &operator-=(const Vec3 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
  }

  Vec3 operator*(const T &v) const {
    return Vec3(e[0] * v, e[1] * v, e[2] * v);
  }
  Vec3 &operator*=(const T t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  Vec3 operator/(const T &t) const {
    assert(t != 0);
    return *this * (1 / t);
  }
  Vec3 &operator/=(const T t) {
    assert(t != 0);
    return *this *= 1 / t;
  }

  static Vec3 random() {
    return Vec3(random_draw<T>(), random_draw<T>(), random_draw<T>());
  }

  static Vec3 random(T min, T max) {
    return Vec3(random_draw<T>(min, max), random_draw<T>(min, max),
                random_draw<T>(min, max));
  }

  bool near_zero() const {
    const auto s = 1e-8;
    return (fabs(e[0] < s) && fabs(e[1] < s) && fabs(e[2] < s));
  }

  double length() const { return sqrt(length_squared()); }

  double length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  template <typename U>
  friend inline std::ostream &operator<<(std::ostream &out, const Vec3<U> &v);

public:
  T e[3];
};

template <typename U>
inline std::ostream &operator<<(std::ostream &out, const Vec3<U> &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

template <typename T> inline Vec3<T> operator*(const T &v, const Vec3<T> &vec) {
  return vec * v;
}

template <typename T> inline T dot(const Vec3<T> &v, const Vec3<T> &u) {
  return v * u;
}

template <typename T> inline T operator*(const Vec3<T> &v, const Vec3<T> &u) {
  return v.x() * u.x() + v.y() * u.y() + v.z() * u.z();
}

template <typename T> inline Vec3<T> cross(const Vec3<T> &v, const Vec3<T> &u) {
  return Vector<T>(v.y() * u.z() - v.z() * u.y(), v.z() * u.x() - v.x() * u.z(),
                   v.x() * u.y() - v.y() * u.x());
}

template <typename T> inline Vec3<T> unit_vector(const Vec3<T> &v) {
  return v / v.length();
}

template <typename T> inline Vec3<T> random_in_unit_sphere() {
  while (true) {
    auto p = Vec3<T>::random(-1, 1);
    if (p.length() >= 1)
      continue;
    return p;
  }
}

template <typename T> inline Vec3<T> random_unit_vector() {
  return unit_vector<T>(random_in_unit_sphere<T>());
}

template <typename T>
inline Vec3<T> reflect(const Vec3<T> &v, const Vec3<T> &n) {
  return v - 2 * dot(v, n) * n;
}

// Type aliases for vec3
using DataType = double;
using Point = Vec3<DataType>;
using Vector = Vec3<DataType>;
using Color = Vec3<DataType>;

} // namespace raytracer

#endif
