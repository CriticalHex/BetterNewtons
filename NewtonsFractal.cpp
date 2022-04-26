#include<iostream>
#include<math.h>
#include<SFML\Graphics.hpp>
#include<complex>
#include<thread>
#include<vector>
#include<list>

using namespace std;

class Pixel { 
public:		  
	Pixel(complex<double> loc, int pos[2]) { 
		c = loc; 
		position[0] = pos[0];
		position[1] = pos[1];
	}
	~Pixel() {}
	void iterate(sf::Image& image) {
		checkRoots();
		image.setPixel(position[0], position[1], color);
		c = newtonsMethod();
	}
	complex<double> c;
private:
	int position[2];
	sf::Color color;

	complex<double> newtonsMethod() {
		return (c - (f(c) / df(c)));
	}

	complex<double> f(complex<double> x) {
		return pow(x, 8) + (real(3) * pow(x, 4)) - real(4);
		//return pow(x, 3) - real(1);
	}

	complex<double> df(complex<double> x) {
		return (real(8) * pow(x, 7)) + (real(12) * pow(x, 2));
		//return real(3) * pow(x, 2);
	}

	void checkRoots() {
		double distanceToP1;
		double distanceToP2;
		double distanceToP3;
		double distanceToP4;
		double distanceToP5;
		double distanceToP6;
		double distanceToP7;
		double distanceToP8;

		//for 3 point function
		/*complex<double> root1(1, 0);
		complex<double> root2(-.5, -.866025);
		complex<double> root3(-.5, .866025);*/

		//for 8 point function
		complex<double> root1(1, 0);
		complex<double> root2(-1, 0);
		complex<double> root3(1, 1);
		complex<double> root4(1, -1);
		complex<double> root5(0, 1);
		complex<double> root6(0, -1);
		complex<double> root7(-1, 1);
		complex<double> root8(-1, -1);

		distanceToP1 = distance(c, root1);
		distanceToP2 = distance(c, root2);
		distanceToP3 = distance(c, root3);
		distanceToP4 = distance(c, root4);
		distanceToP5 = distance(c, root5);
		distanceToP6 = distance(c, root6);
		distanceToP7 = distance(c, root7);
		distanceToP8 = distance(c, root8);

		//for 3 point function
		/*if (distanceToP1 < distanceToP2 and distanceToP1 < distanceToP3) {
			color = sf::Color(104, 129, 157);
		}
		else if (distanceToP2 < distanceToP1 and distanceToP2 < distanceToP3) {
			color = sf::Color(173, 189, 56);
		}
		else if (distanceToP3 < distanceToP2 and distanceToP3 < distanceToP1) {
			color = sf::Color(89, 130, 52);
		}*/

		color = sf::Color(c.imag() * 6, c.imag() * 8, c.imag() * 12);

//for 8 point function
if (distanceToP1 == min({ distanceToP1, distanceToP2, distanceToP3, distanceToP4, distanceToP5, distanceToP6, distanceToP7, distanceToP8 })) {
	color = sf::Color::Blue;
}
else if (distanceToP2 == min({ distanceToP1, distanceToP2, distanceToP3, distanceToP4, distanceToP5, distanceToP6, distanceToP7, distanceToP8 })) {
	color = sf::Color::Cyan;
}
else if (distanceToP3 == min({ distanceToP1, distanceToP2, distanceToP3, distanceToP4, distanceToP5, distanceToP6, distanceToP7, distanceToP8 })) {
	color = sf::Color::Green;
}
else if (distanceToP4 == min({ distanceToP1, distanceToP2, distanceToP3, distanceToP4, distanceToP5, distanceToP6, distanceToP7, distanceToP8 })) {
	color = sf::Color::Magenta;
}
else if (distanceToP5 == min({ distanceToP1, distanceToP2, distanceToP3, distanceToP4, distanceToP5, distanceToP6, distanceToP7, distanceToP8 })) {
	color = sf::Color::Red;
}
else if (distanceToP6 == min({ distanceToP1, distanceToP2, distanceToP3, distanceToP4, distanceToP5, distanceToP6, distanceToP7, distanceToP8 })) {
	color = sf::Color::Yellow;
}
else if (distanceToP7 == min({ distanceToP1, distanceToP2, distanceToP3, distanceToP4, distanceToP5, distanceToP6, distanceToP7, distanceToP8 })) {
	color = sf::Color(89, 130, 52);
}
else if (distanceToP8 == min({ distanceToP1, distanceToP2, distanceToP3, distanceToP4, distanceToP5, distanceToP6, distanceToP7, distanceToP8 })) {
	color = sf::Color(173, 189, 56);
}
	}

	double distance(complex<double> pos1, complex<double> pos2) {
		return (pow(pos2.real() - pos1.real(), 2) + pow(pos2.imag() - pos1.imag(), 2));
	}
};



void fill_array(vector<Pixel*>& pixels, long double scale, int width, int height, sf::Vector2f mouse) { //fills the pixel array with Pixel objects
	long double shift[2] = { 0, 0 };
	long double horizontalStart;
	long double horizontalEnd;
	long double verticalStart;
	long double verticalEnd;
	long double origin[2] = { (width / 2), (height / 2) };
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
	int position[2] = { 0, 0 };

	for (long double t = horizontalStart; t < horizontalEnd; t += horizontalRes) {

		for (long double m = verticalStart; m < verticalEnd; m += verticalRes) {

			position[0] = (t * horizontalSize) + shift[0];
			position[1] = (m * verticalSize) + shift[1];

			if (position[0] >= 0 and position[0] <= width and position[1] >= 0 and position[1] <= height) {

				c._Val[0] = t;
				c._Val[1] = m;

				pixels.push_back(new Pixel(c, position));
			}
		}
	}
}

void draw(vector<Pixel*>& pixels, sf::Image& image, int index_begin, int index_end) {
	for (int i = index_begin; i < index_end; i++) {
		pixels[i]->iterate(ref(image));
	}
}

vector<thread> create_threads(vector<Pixel*>& pixels, sf::Image& image, int max) {
	vector<thread> threads;
	int section = pixels.size() / max;;
	for (int i = 0; i < max; i++) {
		threads.emplace_back(draw, ref(pixels), ref(image), section * i, (section * (i + 1)) - 1);
	}
	return threads;
}

void findRoots(vector<Pixel*>& pixels) {
	vector<complex<double>> coords;
	vector<complex<double>> roots;
	bool append;
	for (int i = 0; i < pixels.size(); i++) {
		coords.push_back(pixels[i]->c);
	}
	for (const auto& num : coords) {
		append = false;
		if (roots.size() == 0) {
			roots.push_back(num);
		}
		for (const auto& r : roots) {
			if (num != r) {
				append = true;
			}
			else {
				append = false;
				break;
			}
		}
		if (append) {
			roots.push_back(num);
		}
	}
	for (const auto& i : roots)
		std::cout << i.real() << ", " << i.imag() << endl;
	cout << endl << endl;
}


int main() {
	//RENDER SETUP----------------------------------------------------------
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Newtons Fractal", sf::Style::None);
	window.setPosition(sf::Vector2i(460, 20));

	//VARIABLES-------------------------------------------------------------
	sf::Color bgColor = sf::Color(0,0,0); 
	int zooms = 0; //max 44 before error shows up, hopefully a future fix
	long double scale = pow(2, zooms);

	sf::Event event;
	vector<thread> active_threads;
	int max_threads = 12;
	sf::Image image;
	image.create(window.getSize().x, window.getSize().y, bgColor);
	sf::Texture texture; 
	texture.loadFromImage(image);
	sf::Sprite newtons(texture);
	vector<Pixel*> pixels;
	sf::CircleShape dot(1);
	dot.setFillColor(sf::Color::Green);
	dot.setPosition(500, 500);

	//INITIAL SET CREATION---------------------------------------------------
	fill_array(ref(pixels), scale, window.getSize().x, window.getSize().y, sf::Vector2f(500, 500));

	//GAME LOOP--------------------------------------------------------------
	while (window.isOpen()) {
		while (window.pollEvent(event))
		{
			// Close window: exit-----------------
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				window.close();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				findRoots(ref(pixels));
			}

			if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.delta > 0) {
					scale *= 2;
					zooms += 1;
				}
				else if (event.mouseWheelScroll.delta < 0) {
					scale /= 2;
					zooms -= 1;
				}
				pixels.clear();
				fill_array(ref(pixels), scale, window.getSize().x, window.getSize().y, sf::Vector2f(event.mouseWheelScroll.x, event.mouseWheelScroll.y));
				//cout << zooms << endl;
				//cout << event.mouseWheelScroll.x << ", " << event.mouseWheelScroll.y << endl; 
				//event.mouseWheelScroll.x, event.mouseWheelScroll.y //use this for mouse control when zooming

			}
		}
		//RENDER--------------------------------------------------------------
		window.clear(bgColor);

		active_threads = create_threads(ref(pixels), ref(image), max_threads);
		for (auto& th : active_threads) {
			if (th.joinable()) {
				th.join();
			}
			th.~thread();
		}
		active_threads.clear();

		texture.loadFromImage(image);
		window.draw(newtons);

		window.draw(dot);
		window.display();
	}
}