#pragma once

#include "Pch.h"

#include "vec3d.h"

namespace util {
constexpr auto inifinity = std::numeric_limits<double>::infinity();

constexpr auto pi = 3.14159265359;
constexpr auto deg_to_rad_const = pi / 180.;

inline double deg_to_rad(double deg) { return deg * deg_to_rad_const; }
inline bool is_between(double x, double min, double max) {
  return min < x && x < max;
}

inline double get_random() {
  static std::uniform_real_distribution<double> distribution(0., 1.);
  static std::mt19937 engine;
  return distribution(engine);
}

inline double get_random(double min, double max) {
  return min + get_random() * (max - min);
}

inline vec3d get_random_vec() {
  return vec3d(get_random(), get_random(), get_random());
}

inline vec3d get_random_vec(double min, double max) {
  return vec3d(get_random(min, max), get_random(min, max),
               get_random(min, max));
}

inline vec3d get_random_vec_in_unit_sphere() {
  while (true) {
    const auto vec_in_unit_box = get_random_vec(-1., 1.);
    if (vec_in_unit_box.length_squared() < 1.) return vec_in_unit_box;
  }
}

inline void cout_vec(std::string const& name, vec3d v) {
  std::cerr << name << ": " << v.x << " " << v.y << " " << v.z << '\n';
}

}  // namespace util
