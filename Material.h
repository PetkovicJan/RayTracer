#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Utils.h"

class Material {
 public:
  virtual ~Material() {}

  virtual bool scatterRay(Ray const& ray_in, HitRecord const& hit_record,
                          Ray& ray_out, Color& albedo) const = 0;
};

// Diffuse material, which sends the scattered ray in quasi-random direction.
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
    const auto new_ray_dir =
        hit_record.normal + util::get_random_vec_in_unit_sphere();
    ray_out = Ray(hit_record.point, new_ray_dir);

    albedo = albedo_;

    return true;
  }

 private:
  Color albedo_;
};
