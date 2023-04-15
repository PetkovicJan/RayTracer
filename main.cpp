#include <iostream>

#include "Camera.h"
#include "HittableList.h"
#include "Material.h"
#include "Sphere.h"

void writeColor(std::ostream& out, Color const& pixel_color) {
  // Gamma correct the values.
  const auto r = sqrt(pixel_color.x);
  const auto g = sqrt(pixel_color.y);
  const auto b = sqrt(pixel_color.z);

  // Write the translated [0,255] value of each color component. Note that due
  // to casting, we need to use the interval [0., 256) in order to represent
  // each "bin" equivalently.
  out << static_cast<int>(255.999 * r) << ' ' << static_cast<int>(255.999 * g)
      << ' ' << static_cast<int>(255.999 * b) << '\n';
}

Ray getDiffuseReflectedRay(HitRecord const& hit_record) {
  // Obtain a new random direction by choosing a random point in a ball at the
  // point of hit, shifted by a normal. This generates a certain distribution of
  // reflected rays with higher probability in the direction of normal and small
  // probability at shallow angles. Note that the direction is independent of
  // the incomming ray direction. There are many different choices of generating
  // new light rays, for example choosing the vectors only on the sphere surface
  // or in the upper hemisphere of the sphere centered at hit point...
  const auto new_ray_dir =
      hit_record.normal + util::get_random_vec_in_unit_sphere();

  return Ray(hit_record.point, new_ray_dir);
}

Color getRayColor(HittableList const& world, Ray const& ray, int depth) {
  // If ray scattered too many times, simply stop it to prevent too deep
  // recursions.
  if (depth <= 0) return Color(0., 0., 0.);

  // The reflected ray by definition hits the object it is reflecting off at 0 +
  // some rounding error. In order to avoid those solutions, we simply use some
  // small t_min > 0.
  HitRecord hit_record;
  if (world.hit(ray, 0.001, util::inifinity, hit_record)) {
    Ray scattered_ray;
    Color albedo;
    if (hit_record.material->scatterRay(ray, hit_record, scattered_ray,
                                        albedo)) {
      return albedo * getRayColor(world, scattered_ray, depth - 1);
    } else {
      return Color();
    }
  }

  // Ranges from -1 to 1.
  const auto y = unit_vector(ray.direction()).y;

  // Ranges from 0 to 1.
  const auto y1 = 0.5 * (y + 1.);

  return (1. - y1) * Color(1., 1., 1.) + y1 * Color(0.5, 0.7, 1.0);
}

// We generate images with ray tracing in the following way. We set up the
// "world" - a set of objects in 3D. Then we set up the camera eye (a point in
// 3D space) and a window (a rectangle in 3D space), through which the eye sees
// the world. The window is divided into a mesh, that represents the pixels,
// which we want to generate. In order to obtain the color of a given pixel, we
// backtrace the ray path from the eye through that pixel and determine its
// color based on what it has hit on its way towards the eye. Note that the ray
// may reflect, refract, get partially absorbed, etc. depending on the objects
// it hits on the way.
int main(int argc, char* argv[]) {
  // Choose appropriate aspect ratio (width over height).
  const auto aspect_ratio = 16. / 9.;
  const auto img_width = 400;
  const auto img_height = static_cast<int>(img_width / aspect_ratio);
  const Camera cam(img_width, img_height);

  const int num_samples_per_pixel = 50;

  const int max_ray_depth = 50;

  // Materials.
  Lambertian material_lambertian(Color(0.5, 0.5, 0.5));

  // Setup the world.
  HittableList world;
  world.add<Sphere>(vec3d(0., 0., 3.), 1., &material_lambertian);
  world.add<Sphere>(vec3d(0., 101.5, 1.), 100., &material_lambertian);

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
