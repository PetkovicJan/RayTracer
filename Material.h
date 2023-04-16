#pragma once

#include "Pch.h"

#include "Hittable.h"
#include "Ray.h"
#include "Utils.h"

class Material {
 public:
  virtual ~Material() {}

  virtual bool scatterRay(Ray const& ray_in, HitRecord const& hit_record,
                          Ray& ray_out, Color& albedo) const = 0;
};

// Diffuse material reflects the scattered ray in quasi-random direction.
class Lambertian : public Material {
 public:
  explicit Lambertian(Color const& albedo) : albedo_(albedo) {}

  bool scatterRay(Ray const& ray_in, HitRecord const& hit_record, Ray& ray_out,
                  Color& albedo) const override {
    // Obtain a new random direction by choosing a random point in a ball at the
    // point of hit, shifted by a normal. This generates a certain distribution
    // of reflected rays with higher probability in the direction of normal and
    // small probability at shallow angles. Note that the direction is
    // independent of the incomming ray direction. There are many different
    // choices of generating new light rays, for example choosing the vectors
    // only on the sphere surface or in the upper hemisphere of the sphere
    // centered at hit point...
    auto new_ray_dir =
        hit_record.normal + util::get_random_vec_in_unit_sphere();
    // Handle the case when the new ray direction is very close to zero.
    if (new_ray_dir.length_squared() < 1e-8) {
      new_ray_dir = hit_record.normal;
    }

    ray_out = Ray(hit_record.point, unit_vector(new_ray_dir));

    albedo = albedo_;

    return true;
  }

 private:
  Color albedo_;
};

// Metal material reflects the ray at the same angle as incomming.
class Metal : public Material {
 public:
  explicit Metal(Color const& albedo, double fuzziness)
      : albedo_(albedo), fuzziness_(fuzziness) {}

  bool scatterRay(Ray const& ray_in, HitRecord const& hit_record, Ray& ray_out,
                  Color& albedo) const override {
    const auto& dir_in = ray_in.direction();
    const auto& normal = hit_record.normal;

    const auto reflected =
        unit_vector(dir_in - 2. * dot(dir_in, normal) * normal);
    const auto scattered = unit_vector(
        reflected + fuzziness_ * util::get_random_vec_in_unit_sphere());

    ray_out = Ray(hit_record.point, scattered);

    albedo = albedo_;

    return true;
  }

 private:
  Color albedo_;
  double fuzziness_;
};

namespace detail {
//  Reflectance is the probability of a ray to reflect in a dielectric material
//  (instead of refracting) and varies with angle of the incomming ray as well
//  as with the ratio of refraction indices of both materials.
inline double reflectance(double cosine, double refraction_ratio) {
  // A common Schlick's approximation.
  auto r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}
}  // namespace detail

// Dielectric material refracts the ray through Snell's law.
class Dielectric : public Material {
 public:
  explicit Dielectric(double refraction_index)
      : refraction_index_(refraction_index),
        inv_refraction_index_(1. / refraction_index) {}

  bool scatterRay(Ray const& ray_in, HitRecord const& hit_record, Ray& ray_out,
                  Color& albedo) const override {
    const auto unit_dir_in = unit_vector(ray_in.direction());
    const auto& normal = hit_record.normal;

    const auto cos_alpha = -dot(unit_dir_in, normal);
    const auto sin_alpha = sqrt(1. - cos_alpha * cos_alpha);

    const auto refraction_factor =
        hit_record.front_face ? inv_refraction_index_ : refraction_index_;

    const auto can_refract = refraction_factor * sin_alpha < 1.;

    vec3d scattered_dir;
    if (can_refract && (util::get_random() >
                        detail::reflectance(cos_alpha, refraction_factor))) {
      const auto unit_dir_out_perpendicular =
          refraction_factor * (unit_dir_in + cos_alpha * normal);
      const auto unit_dir_out_parallel =
          -1. * sqrt(1. - unit_dir_out_perpendicular.length_squared()) * normal;

      scattered_dir = unit_dir_out_perpendicular + unit_dir_out_parallel;
    } else {
      // Reflect the ray.
      scattered_dir =
          unit_vector(unit_dir_in - 2. * dot(unit_dir_in, normal) * normal);
    }

    ray_out = Ray(hit_record.point, scattered_dir);
    albedo = Color(1., 1., 1.);

    return true;
  }

 private:
  double refraction_index_;
  double inv_refraction_index_;
};
