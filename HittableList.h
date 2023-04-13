#pragma once

#include <memory>
#include <type_traits>
#include <vector>

#include "Hittable.h"

class HittableList : public Hittable {
 public:
  void clear() { hittables_.clear(); }

  template <typename HittableT,
            class... Args,
            typename = std::enable_if_t<std::is_base_of_v<Hittable, HittableT>>>
  void add(Args&&... args) {
    hittables_.push_back(
        std::make_unique<HittableT>(std::forward<Args>(args)...));
  }

  bool hit(Ray const& ray, double t_min, double t_max,
           HitRecord& hit_record) const override {
    // Find the closest hit.
    HitRecord current_hr;
    bool any_hit = false;
    auto t_closest = t_max;
    for (auto const& hittable : hittables_) {
      if (hittable->hit(ray, t_min, t_closest, current_hr)) {
        any_hit = true;
        hit_record = current_hr;
        t_closest = current_hr.t;
      }
    }

    return any_hit;
  }

 private:
  std::vector<std::unique_ptr<Hittable>> hittables_;
};
