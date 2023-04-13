#pragma once

#include <limits>
#include <random>

#include "vec3d.h"

namespace util {
constexpr auto inifinity = std::numeric_limits<double>::infinity();

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

}  // namespace util
