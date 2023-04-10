#include <iostream>

#include "vec3d.h"

using Color = vec3d;

void writeColor(std::ostream& out, Color const& pixel_color) {
  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(255.999 * pixel_color.x) << ' '
      << static_cast<int>(255.999 * pixel_color.y) << ' '
      << static_cast<int>(255.999 * pixel_color.z) << '\n';
}

int main(int argc, char* argv[]) {
  const auto img_width = 256;
  const auto img_height = 256;

  // Output image in ppm format. Note that we can redirect the output to a file
  // with .ppm extension using this command: app.exe > image.ppm
  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
  for (int i = 0; i < img_height; ++i) {
    for (int j = 0; j < img_width; ++j) {
      const auto r = double(j) / img_width;
      const auto g = double(i) / img_height;
      const auto b = 0.25;

      writeColor(std::cout, Color(r, g, b));
    }
  }

  return 0;
}
