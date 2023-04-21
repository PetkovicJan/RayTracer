#pragma once

#include "vec3d.h"

class Ray {
 public:
  explicit Ray() {}

  explicit Ray(vec3d const& origin, vec3d const& dir)
      : origin_(origin), dir_(dir) {}

  vec3d origin() const { return origin_; }

  vec3d direction() const { return dir_; }

  vec3d at(double t) const { return origin_ + t * dir_; }

 private:
  vec3d origin_;
  vec3d dir_;
};
