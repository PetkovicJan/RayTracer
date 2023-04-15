#pragma once

#include "Ray.h"

class Material;

struct HitRecord {
  // Point of ray hit.
  vec3d point;

  // Normal to the surface at the point of the hit on the side of the incomming ray.
  vec3d normal;

  // Parameter of the ray at the point of the hit.
  double t;

  // Is hit at the front?
  bool front_face;

  // Material at the point of the hit.
  Material* material = nullptr;

  inline void set_face_normal(Ray const& ray, vec3d const& outward_normal) {
    front_face = dot(ray.direction(), outward_normal) < 0.;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
 public:
  virtual bool hit(Ray const& ray, double t_min, double t_max,
                   HitRecord& hit_record) const = 0;

  virtual ~Hittable() {}
};
