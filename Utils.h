#pragma once

#include <limits>

namespace util {
constexpr auto inifinity = std::numeric_limits<double>::infinity();

inline bool is_between(double x, double min, double max) {
  return min < x && x < max;
}
}  // namespace util
