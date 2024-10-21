#ifndef PIXEL_H
#define PIXEL_H

#include <SFML/Graphics.hpp>
#include <complex>

using namespace std;

class Pixel {
public:
  Pixel(complex<double> loc, int pos[2]);
  ~Pixel() {}

  void iterate(sf::Image &image);

private:
  bool found = false;
  int count = 0;
  int position[2];
  sf::Color color = sf::Color(255, 255, 255);
  complex<double> z;
  // complex<double> a;
  // complex<double> z = a / real(3);

  complex<double> newtonsMethod(int maxIter);

  complex<double> f(complex<double> x);

  complex<double> df(complex<double> x);
};

#endif