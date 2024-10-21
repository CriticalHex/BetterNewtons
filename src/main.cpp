#include "pixel.h"
#include <iostream>
#include <math.h>
#include <thread>
#include <vector>

/*
Eight root
function:-------------------------------------------------------------
complex<double> f(complex<double> x) {
        return (pow(x, 8) + (real(15) * pow(x, 4)) - real(16));

}

complex<double> df(complex<double> x) {
        return ((real(8) * pow(x, 7)) + (real(60) * pow(x, 3)));
}


Supposed mandelbrot function: requires some program restructuring...
------------- complex<double> f(complex<double> x) { return ((pow(x, 2) +
real(1)) * (x - a));
}

complex<double> df(complex<double> x) {
        return (((real(2) * x) * (x - a)) + (pow(x, 2) + real(1)));
}
*/

using namespace std;

void fill_array(
    vector<Pixel *> &pixels, long double scale, int width, int height,
    sf::Vector2f mouse) { // fills the pixel array with Pixel objects
  long double shift[2] = {0, 0};
  long double horizontalStart;
  long double horizontalEnd;
  long double verticalStart;
  long double verticalEnd;
  long double origin[2] = {(width / 2.), (height / 2.)};
  long double scaleStart = (-2 / scale);
  long double scaleEnd = (2 / scale);
  long double horizontalSize = (width / (abs(scaleStart) + abs(scaleEnd)));
  long double verticalSize = (height / (abs(scaleStart) + abs(scaleEnd)));
  long double horizontalRes = (abs(scaleStart) + abs(scaleEnd)) / width * 1;
  long double verticalRes = (abs(scaleStart) + abs(scaleEnd)) / height * 1;

  shift[0] = origin[0] + ((origin[0] - mouse.x) * scale);
  shift[1] = origin[1] + ((origin[1] - mouse.y) * scale);

  horizontalStart = scaleStart + ((origin[0] - shift[0]) / horizontalSize);
  horizontalEnd = scaleEnd + ((origin[0] - shift[0]) / horizontalSize);
  verticalStart = scaleStart + ((origin[1] - shift[1]) / verticalSize);
  verticalEnd = scaleEnd + ((origin[1] - shift[1]) / verticalSize);

  complex<double> c;
  int position[2] = {0, 0};

  for (long double t = horizontalStart; t < horizontalEnd; t += horizontalRes) {

    for (long double m = verticalStart; m < verticalEnd; m += verticalRes) {

      position[0] = (t * horizontalSize) + shift[0];
      position[1] = (m * verticalSize) + shift[1];

      if (position[0] >= 0 && position[0] <= width && position[1] >= 0 &&
          position[1] <= height) {

        c.real(t);
        c.imag(m);

        pixels.push_back(new Pixel(c, position));
      }
    }
  }
}

void draw(vector<Pixel *> &pixels, sf::Image &image, int index_begin,
          int index_end) {
  for (int i = index_begin; i < index_end; i++) {
    pixels[i]->iterate(ref(image));
  }
}

vector<thread> create_threads(vector<Pixel *> &pixels, sf::Image &image,
                              int max) {
  vector<thread> threads;
  int section = pixels.size() / max;
  for (int i = 0; i < max; i++) {
    threads.emplace_back(draw, ref(pixels), ref(image), section * i,
                         (section * (i + 1)) - 1);
  }
  return threads;
}

int main() {
  // RENDER SETUP----------------------------------------------------------
  sf::RenderWindow window(sf::VideoMode(1000, 1000), "Newtons Fractal",
                          sf::Style::None);
  window.setPosition(sf::Vector2i(460, 20));
  sf::Vector2f center(window.getSize().x / 2, window.getSize().y / 2);

  // VARIABLES-------------------------------------------------------------
  sf::Color bgColor = sf::Color(0, 0, 0);
  int zooms = 0; // max 44 before error shows up, hopefully a future fix
  long double scale = pow(2, zooms);

  sf::Event event;
  vector<thread> active_threads;
  int max_threads = 12;
  sf::Image image;
  image.create(window.getSize().x, window.getSize().y, bgColor);
  sf::Texture texture;
  texture.loadFromImage(image);
  sf::Sprite newtons(texture);
  vector<Pixel *> pixels;
  sf::CircleShape dot(1);
  dot.setFillColor(sf::Color::Green);
  dot.setPosition(center.x, center.y);

  // INITIAL SET CREATION---------------------------------------------------
  fill_array(ref(pixels), scale, window.getSize().x, window.getSize().y,
             center);
  active_threads = create_threads(ref(pixels), ref(image), max_threads);
  for (auto &th : active_threads) {
    if (th.joinable()) {
      th.join();
    }
    th.~thread();
  }
  active_threads.clear();

  // GAME LOOP--------------------------------------------------------------
  while (window.isOpen()) {
    while (window.pollEvent(event)) {
      // Close window: exit-----------------
      if (event.type == sf::Event::Closed ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        window.close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
        image.saveToFile("screenshot.png");
      }

      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0) {
          scale *= 2;
          zooms += 1;
        } else if (event.mouseWheelScroll.delta < 0) {
          scale /= 2;
          zooms -= 1;
        }
        pixels.clear();
        // fill_array(ref(pixels), scale, window.getSize().x,
        // window.getSize().y, sf::Vector2f(event.mouseWheelScroll.x,
        // event.mouseWheelScroll.y));
        fill_array(
            ref(pixels), scale, window.getSize().x, window.getSize().y,
            sf::Vector2f(event.mouseWheelScroll.x, event.mouseWheelScroll.y));
        // event.mouseWheelScroll.x, event.mouseWheelScroll.y //mouse postiion
        // on scroll
        active_threads = create_threads(ref(pixels), ref(image), max_threads);
        for (auto &th : active_threads) {
          if (th.joinable()) {
            th.join();
          }
          th.~thread();
        }
        active_threads.clear();
      }
    }
    // RENDER--------------------------------------------------------------
    window.clear(bgColor);

    texture.loadFromImage(image);
    window.draw(newtons);
    // cout << sizeof(texture) << "\n";

    window.draw(dot);
    window.display();
  }
}