#include <iostream>

#include "HittableList.h"
#include "Sphere.h"

using Color = vec3d;

void writeColor(std::ostream& out, Color const& pixel_color) {
  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(255.999 * pixel_color.x) << ' '
      << static_cast<int>(255.999 * pixel_color.y) << ' '
      << static_cast<int>(255.999 * pixel_color.z) << '\n';
}

Color getRayColor(HittableList const& world, Ray const& ray) {
  // Check if sphere is hit and return red in that case.
  HitRecord hit_record;
  if (world.hit(ray, 0., util::inifinity, hit_record)) {
    // Use normal to set the color. Linearly transform values to range from 0
    // to 1.
    return 0.5 * (hit_record.normal + vec3d(1., 1., 1.));
  }

  // Ranges from -1 to 1.
  const auto y = unit_vector(ray.direction()).y;

  // Ranges from 0 to 1.
  const auto y1 = 0.5 * (y + 1.);

  return (1. - y1) * Color(1., 1., 1.) + y1 * Color(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[]) {
  // Width over height.
  const auto aspect_ratio = 16. / 9.;

  const auto img_width = 400;
  const auto img_height = static_cast<int>(img_width / aspect_ratio);

  // Setup the "eye" (origin) and the viewport, that maps to the image.
  // The "eye sits at the origin. The viewport is a rectangle with the
  // same aspect ratio as the image and sits at the distance one along
  // the z-axis. We choose its height to be 2.
  const auto origin = vec3d(0., 0., 0.);

  const auto viewport_height = 2.0;
  const auto viewport_width = viewport_height * aspect_ratio;
  const auto focal_length = 1.0;

  const auto horizontal = vec3d(viewport_width, 0., 0.);
  const auto vertical = vec3d(0., viewport_height, 0.);
  const auto viewport_origin =
      vec3d(0., 0., focal_length) - horizontal / 2. - vertical / 2.;

  // Setup the world.
  HittableList world;
  world.add<Sphere>(vec3d(0., 0., 3.), 1.);
  world.add<Sphere>(vec3d(0., 101.5, 1.), 100.);

  // Output image in ppm format. Note that we can redirect the output to a file
  // with .ppm extension using this command: app.exe > image.ppm
  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
  for (int i = 0; i < img_height; ++i) {
    for (int j = 0; j < img_width; ++j) {
      const auto x = double(j) / img_width;
      const auto y = double(i) / img_height;

      const auto viewport_pos = viewport_origin + x * horizontal + y * vertical;
      const auto ray = Ray(origin, viewport_pos - origin);

      const auto ray_color = getRayColor(world, ray);
      writeColor(std::cout, ray_color);
    }
  }

  return 0;
}
