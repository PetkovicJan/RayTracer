#pragma once

#include <cmath>

struct vec3d {
  double x{0.};
  double y{0.};
  double z{0.};

  explicit vec3d() {}
  explicit vec3d(double x, double y, double z) : x(x), y(y), z(z) {}

  vec3d operator-() const { return vec3d(-x, -y, -z); }

  vec3d& operator+=(vec3d const& other) {
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
  }

  vec3d& operator-=(vec3d const& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
  }

  vec3d& operator*=(double t) {
    x *= t;
    y *= t;
    z *= t;

    return *this;
  }

  vec3d& operator/=(double t) {
    (*this) *= 1 / t;

    return *this;
  }

  double length_squared() const { return x * x + y * y + z * z; }

  double length() const { return sqrt(this->length_squared()); }
};

// Utility functions.
inline vec3d operator+(vec3d const& v, vec3d const& u) {
  return vec3d(v.x + u.x, v.y + u.y, v.z + u.z);
}

inline vec3d operator-(vec3d const& v, vec3d const& u) {
  return vec3d(v.x - u.x, v.y - u.y, v.z - u.z);
}

inline vec3d operator*(double t, vec3d const& v) {
  return vec3d(t * v.x, t * v.y, t * v.z);
}

inline vec3d operator*(vec3d const& v, double t) { return t * v; }

inline vec3d operator/(vec3d const& v, double t) { return (1. / t) * v; }

inline double dot(vec3d const& v, vec3d const& u) {
  return v.x * u.x + v.y * u.y + v.z * u.z;
}

inline vec3d unit_vector(vec3d const& v) {
  return v / v.length();
}