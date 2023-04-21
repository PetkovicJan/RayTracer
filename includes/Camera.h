#pragma once

#include "Ray.h"
#include "Utils.h"

class Camera {
 public:
  explicit Camera(vec3d eye, vec3d direction, double vertical_fov, int width,
                  int height)
      : eye_(eye), width_(width), height_(height) {
    const auto aspect_ratio = double(width) / height;

    // This choice is arbitrary. Viewport can be translated along the
    // direction of view and scaled for the same factor and we obtain the same
    // result.
    const auto viewport_height = 1.0;

    const auto viewport_width = viewport_height * aspect_ratio;

    // Distance between the eye and the viewport is determined by the required
    // field-of-view. The closer the eye is to the viewport, the larger the
    // "fov".
    const auto focal_length = viewport_height / (2. * tan(vertical_fov / 2.));

    // In general there is no canonical orientation of the viewport. We choose
    // it to make the horizontal vector perpendicular to the y direction.
    const auto y_axis = vec3d(0., 1., 0.);
    horizontal_ = viewport_width * unit_vector(cross(y_axis, direction));
    vertical_ = viewport_height * unit_vector(cross(direction, horizontal_));

    const auto viewport_center = eye + focal_length * unit_vector(direction);
    viewport_origin_ = viewport_center - horizontal_ / 2. - vertical_ / 2.;
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
