#pragma once

#include "Ray.h"

struct HitRecord {
  vec3d point;
  vec3d normal;
  double t;
  bool front_face;

  inline void set_face_normal(Ray const& ray, vec3d const& outward_normal) {
    front_face = dot(ray.direction(), outward_normal) < 0.;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Hittable {
 public:
  virtual bool hit(Ray const& ray, double t_min, double t_max,
                   HitRecord& hit_record) const = 0;

  virtual ~Hittable(){}
};
