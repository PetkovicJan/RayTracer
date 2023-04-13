#include <iostream>

#include "Camera.h"
#include "HittableList.h"
#include "Sphere.h"

using Color = vec3d;

void writeColor(std::ostream& out, Color const& pixel_color) {
  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(255.999 * pixel_color.x) << ' '
      << static_cast<int>(255.999 * pixel_color.y) << ' '
      << static_cast<int>(255.999 * pixel_color.z) << '\n';
}

Color getRayColor(HittableList const& world, Ray const& ray, int depth) {
  if (depth <= 0) return Color();


  HitRecord hit_record;
  if (world.hit(ray, 0., util::inifinity, hit_record)) {
    const auto new_ray_dir =
        hit_record.normal + util::get_random_vec_in_unit_sphere();
    const Ray difused_ray(hit_record.point, new_ray_dir);
    constexpr double reflection_factor = 0.5;
    return reflection_factor * getRayColor(world, difused_ray, depth - 1);
  }

  // Ranges from -1 to 1.
  const auto y = unit_vector(ray.direction()).y;

  // Ranges from 0 to 1.
  const auto y1 = 0.5 * (y + 1.);

  return (1. - y1) * Color(1., 1., 1.) + y1 * Color(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[]) {
  // Choose appropriate aspect ratio (width over height).
  const auto aspect_ratio = 16. / 9.;
  const auto img_width = 400;
  const auto img_height = static_cast<int>(img_width / aspect_ratio);
  const Camera cam(img_width, img_height);

  const int num_samples_per_pixel = 50;

  const int max_ray_depth = 50;

  // Setup the world.
  HittableList world;
  world.add<Sphere>(vec3d(0., 0., 3.), 1.);
  world.add<Sphere>(vec3d(0., 101.5, 1.), 100.);

  // Output image in ppm format. Note that we can redirect the output to a file
  // with .ppm extension using this command: app.exe > image.ppm
  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
  for (int row = 0; row < img_height; ++row) {
    for (int col = 0; col < img_width; ++col) {
      Color pixel_color;
      for (int s = 0; s < num_samples_per_pixel; ++s) {
        const auto ray = cam.getRayThroughPixel(row, col);
        pixel_color += getRayColor(world, ray, max_ray_depth);
      }
      pixel_color /= double(num_samples_per_pixel);

      writeColor(std::cout, pixel_color);
    }
  }

  return 0;
}
