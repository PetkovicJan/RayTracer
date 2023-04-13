#pragma once

#include "Ray.h"
#include "Utils.h"

class Camera {
 public:
  explicit Camera(int width, int height) : width_(width), height_(height) {
    const auto aspect_ratio = double(width) / height;

    // Setup the "eye" (origin) and the viewport, that maps to the image.
    // The "eye sits at the origin. The viewport is a rectangle with the
    // same aspect ratio as the image and sits at the distance one along
    // the z-axis. We choose its height to be 2.
    eye_ = vec3d(0., 0., 0.);

    const auto viewport_height = 2.0;
    const auto viewport_width = viewport_height * aspect_ratio;
    const auto focal_length = 1.0;

    horizontal_ = vec3d(viewport_width, 0., 0.);
    vertical_ = vec3d(0., viewport_height, 0.);
    viewport_origin_ =
        vec3d(0., 0., focal_length) - horizontal_ / 2. - vertical_ / 2.;
  }

  Ray getRayThroughPixel(int row, int column) const {
    const auto x = (column + util::get_random()) / width_;
    const auto y = (row + util::get_random()) / height_;

    const auto viewport_pos =
        viewport_origin_ + x * horizontal_ + y * vertical_;

    const auto ray = Ray(eye_, viewport_pos - eye_);

    return ray;
  }

 private:
  int width_ = 0;
  int height_ = 0;

  vec3d eye_;
  vec3d viewport_origin_;
  vec3d horizontal_;
  vec3d vertical_;
};
