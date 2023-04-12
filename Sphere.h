#pragma once

#include "Hittable.h"
#include "Utils.h"

class Sphere : public Hittable {
 public:
  explicit Sphere(vec3d const& center, double radius)
      : center_(center), radius_(radius) {}

  bool hit(Ray const& ray, double t_min, double t_max,
           HitRecord& hit_record) override {
    // Compute the solution of the quadratic equation, obtained from the
    // requirement: |ray(t) - center|^2 = radius^2
    const auto tmp = ray.origin() - center_;

    // Factor of 2 factors out and simplifies the standard quadratic equation.
    const auto a = ray.direction().length_squared();
    const auto half_b = dot(ray.direction(), tmp);
    const auto c = tmp.length_squared() - radius_ * radius_;

    const auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0.) {
      return false;
    }

    // Find the nearest root, that lies in the acceptable range.
    const auto sqrt_discriminant = sqrt(discriminant);
    auto t = (-half_b - sqrt_discriminant) / a;
    if (!util::is_between(t, t_min, t_max)) {
      t = (-half_b + sqrt_discriminant) / a;
      if (!util::is_between(t, t_min, t_max)) {
        return false;
      }
    }

    hit_record.t = t;
    hit_record.point = ray.at(t);
    const auto outward_normal = (hit_record.point - center_) / radius_;
    hit_record.set_face_normal(ray, outward_normal);

    return true;
  }

 private:
  vec3d center_;
  double radius_;
};
