#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <iostream>
#include <map>
#include <thread>

#define SCRWIDTH 400
#define SCRHEIGHT 400
#define OFFSET 10
#define LINE_DETAILS 3.0f
#define PI 3.14159f
#define GMT_DIFF -4

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "Clock", sf::Style::Titlebar | sf::Style::Close, settings);

	sf::CircleShape rim(SCRHEIGHT / 2 - OFFSET, 100);
	rim.setPosition(OFFSET, OFFSET);
	rim.setFillColor(sf::Color(0, 0, 0));
	rim.setOutlineThickness(LINE_DETAILS);
	rim.setOutlineColor(sf::Color(255, 255, 255));

	sf::RectangleShape hour(sf::Vector2f(100, LINE_DETAILS)),
		min(sf::Vector2f(150, LINE_DETAILS)),
		sec(sf::Vector2f(180, LINE_DETAILS));
	hour.setOrigin(0, LINE_DETAILS / 2);
	min.setOrigin(0, LINE_DETAILS / 2);
	sec.setOrigin(0, LINE_DETAILS / 2);
	hour.setPosition(SCRWIDTH / 2, SCRWIDTH / 2);
	min.setPosition(SCRWIDTH / 2, SCRWIDTH / 2);
	sec.setPosition(SCRWIDTH / 2, SCRWIDTH / 2);

	std::map<int, sf::RectangleShape> markers;
	for (int rotation = 0; rotation < 360; rotation += 30) {
		markers[rotation] = sf::RectangleShape(sf::Vector2f(50, LINE_DETAILS));
		sf::RectangleShape& line = markers[rotation];
		line.rotate((float)rotation);
		line.setOrigin(0, LINE_DETAILS / 2);
		line.setPosition(SCRWIDTH/2 - (SCRWIDTH/2 - OFFSET) * cos((rotation)*PI / 180), SCRHEIGHT / 2 - (SCRHEIGHT / 2 - OFFSET) * sin((rotation)*PI/180));
	}



	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		auto time = std::chrono::system_clock::now();
		long long curTimeMilSec = time.time_since_epoch().count() / 10000;

		hour.setRotation(((curTimeMilSec % 43200000 + GMT_DIFF * 3600000) / 43200000.0f) * 360 - 90);
		min.setRotation(((curTimeMilSec % 3600000) / 3600000.0f) * 360 - 90);
		sec.setRotation(((curTimeMilSec % 60000) / 60000.0f) * 360 - 90);

		//Window Drawing
		window.clear(sf::Color(128, 128, 128));
		window.draw(rim);
		for (auto line : markers) {
			window.draw(line.second);
		}
		window.draw(hour);
		window.draw(min);
		window.draw(sec);
		window.display();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return EXIT_SUCCESS;
}
