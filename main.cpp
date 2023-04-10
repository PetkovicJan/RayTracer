#include <iostream>

int main(int argc, char* argv[])
{
  const auto img_width = 256;
  const auto img_height = 256;

  // Output image in ppm format.
  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
  for (int i = 0; i < img_height; ++i)
  {
    for (int j = 0; j < img_width; ++j)
    {
      const auto r = double(j) / img_width;
      const auto g = double(i) / img_height;
      const auto b = 0.25;

      const auto ir = static_cast<int>(r * 255.);
      const auto ig = static_cast<int>(g * 255.);
      const auto ib = static_cast<int>(b * 255.);

      std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }

  return 0;
}