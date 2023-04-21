#include "Pch.h"

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

HittableList createRandomWorld(vec3d const& scene_center) {
  const auto small_radius = 0.2;
  const auto mid_radius = 1.;
  const auto big_radius = 1000.;

  HittableList world;

  auto ground = std::make_unique<Lambertian>(Color(0.5, 0.5, 0.5));
  world.add<Sphere>(scene_center + vec3d(0., big_radius, 0.), big_radius,
                    std::move(ground));

  // Place small spheres on the plane of the big sphere.
  for (int x = -11; x < 11; ++x)
    for (int y = -11; y < 11; ++y) {
      const auto pos =
          scene_center + vec3d(x + 0.8 * util::get_random(), -small_radius,
                               y + 0.8 * util::get_random());
      if ((pos - vec3d(0., -small_radius, pos.z)).length() < mid_radius)
        continue;

      const auto random_material_number = util::get_random();

      std::unique_ptr<Material> material;
      if (random_material_number < 0.5) {
        const auto albedo = util::get_random_vec() * util::get_random_vec();
        material = std::make_unique<Lambertian>(albedo);
      } else if (random_material_number < 0.85) {
        const auto albedo = util::get_random_vec(0.5, 1.);
        const auto fuzz = util::get_random(0., 0.5);
        material = std::make_unique<Metal>(albedo, fuzz);
      } else {
        material = std::make_unique<Dielectric>(1.5);
      }

      world.add<Sphere>(pos, small_radius, std::move(material));
    }

  // Add 3 middle spheres.
  auto material0 = std::make_unique<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add<Sphere>(scene_center + vec3d(0., -mid_radius, -3.), mid_radius, std::move(material0));

  auto material1 = std::make_unique<Dielectric>(1.5);
  world.add<Sphere>(
    scene_center + vec3d(0., -mid_radius, 0.), mid_radius, std::move(material1));

  auto material2 = std::make_unique<Lambertian>(Color(0.4, 0.2, 0.1));
  world.add<Sphere>(
    scene_center + vec3d(0., -mid_radius, 3.), mid_radius, std::move(material2));

  return world;
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
  const auto scene_center = vec3d(0., 0., 6.);

  const auto cam_eye = vec3d(-2.5, -1.5, 0.);
  const auto cam_direction = scene_center - cam_eye;
  const auto cam_vertical_fov = util::deg_to_rad(45.);

  const auto num_samples_per_pixel = 50;
  const auto max_ray_depth = 50;

  // Choose appropriate aspect ratio (width over height).
  const auto aspect_ratio = 16. / 9.;
  const auto img_width = 800;
  const auto img_height = static_cast<int>(img_width / aspect_ratio);
  const Camera cam(cam_eye, cam_direction, cam_vertical_fov, img_width,
                   img_height);

  HittableList world = createRandomWorld(scene_center);

  // Output image in ppm format. Note that we can redirect the output to a file
  // with .ppm extension using this command: app.exe > image.ppm
  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

  const auto percentage_step_size = 5;
  auto last_step = 0;
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
    const auto percentage = 100. * double(row) / img_height;

    const auto current_step =
        static_cast<int>(percentage / percentage_step_size);
    if (current_step > last_step) {
      last_step = current_step;
      std::cerr << "Percentage done: " << last_step * percentage_step_size
                << "%\n";
    }
  }
  std::cerr << "Percentage done: 100%\n";

  std::cin.get();

  return 0;
}
