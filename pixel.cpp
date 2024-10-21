#include "pixel.h"

using namespace std;

Pixel::Pixel(complex<double> loc, int pos[2]) {
  z = loc;
  position[0] = pos[0];
  position[1] = pos[1];
}

void Pixel::iterate(sf::Image &image) {
  z = newtonsMethod(100);
  // color = sf::Color(count * 6, count * 8, count * 12);
  color = sf::Color(count * 6, count * 8, count * 12);
  image.setPixel(position[0], position[1], color);
}

complex<double> Pixel::newtonsMethod(int maxIter) {
  for (int i = 0; i < maxIter; i++) {
    complex<double> dz = f(z) / df(z);
    if (abs(dz) < .00000001) {
      return z;
    }
    count++;
    z -= dz;
  }
  return false;
}

complex<double> Pixel::f(complex<double> x) {
  // return (pow(x, 8) + (real(15) * pow(x, 4)) - real(16));
  return sin(x);
}

complex<double> Pixel::df(complex<double> x) {
  // return ((real(8) * pow(x, 7)) + (real(60) * pow(x, 3)));
  return cos(x);
}