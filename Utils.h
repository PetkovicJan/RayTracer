#pragma once

#include <limits>
#include <random>

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
}  // namespace util
