#ifndef _COLOR
#define _COLOR

#include "vec3.hpp"
#include <iostream>

namespace raytracer::color {
using namespace raytracer::geo;

void write_color(std::ostream &out, geo::color pixel_color) {
  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(255.999 * pixel_color.x()) << ' '
      << static_cast<int>(255.999 * pixel_color.y()) << ' '
      << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}
} // namespace raytracer::color

#endif
