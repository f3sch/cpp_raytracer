#ifndef _VEC3
#define _VEC3

#include <cmath>
#include <ostream>

namespace raytracer::geo {
using std::sqrt;

template <typename T> class vec3 {
public:
  vec3() : point{0, 0, 0} {}
  vec3(T x, T y, T z) : point{x, y, z} {}

  inline T x() const { return point[0]; }
  inline T y() const { return point[1]; }
  inline T z() const { return point[2]; }

  vec3 operator-() const { return vec3(-point[0], -point[1], -point[2]); }
  T operator[](int i) const { return point[i]; }
  T &operator[](int i) { return point[i]; }

  vec3 &operator+=(const vec3 &v) {
    point[0] += v.point[0];
    point[1] += v.point[1];
    point[2] += v.point[2];
    return *this;
  }

  vec3 &operator*=(const T s) {
    point[0] *= s;
    point[1] *= s;
    point[2] *= s;
    return *this;
  }
  vec3 &operator/=(const T s) { return *this *= 1 / s; }

  T len() const { return sqrt(len_sqrt()); }
  T len_sqrt() const {
    return point[0] * point[0] + point[1] * point[1] + point[2] * point[2];
  }

  // Utility functions
  template <typename U>
  friend std::ostream &operator<<(std::ostream &out, const vec3<U> &v) {
    return out << v.x() << " " << v.y() << " " << v.z();
  }

private:
  T point[3];
};

// Utility functions
template <typename T>
inline vec3<T> operator+(const vec3<T> &u, const vec3<T> &v) {
  return vec3<T>(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

template <typename T>
inline vec3<T> operator-(const vec3<T> &u, const vec3<T> &v) {
  return vec3<T>(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

template <typename T>
inline vec3<T> operator*(const vec3<T> &u, const vec3<T> &v) {
  return vec3<T>(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

template <typename T> inline vec3<T> operator*(const T t, const vec3<T> &v) {
  return vec3<T>(t * v.x(), t * v.y(), t * v.z);
}

template <typename T> inline vec3<T> operator*(const vec3<T> &v, const T t) {
  return t * v;
}

template <typename T> inline vec3<T> operator/(const T t, const vec3<T> &v) {
  return (1 / t) * v;
}

template <typename T> inline vec3<T> operator/(const vec3<T> &v, const T t) {
  return v / t;
}

template <typename T> inline T dot(const vec3<T> &u, const vec3<T> &v) {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

template <typename T> inline vec3<T> cross(const vec3<T> &u, const vec3<T> &v) {
  return vec3<T>(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
                 u.x() * v.y() - u.y() * v.x());
}

// Type aliases for vec3
using point3 = vec3<unsigned int>;
using color = vec3<double>;

} // namespace raytracer::geo

#endif
