#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "wtypes.h"
#include <chrono>
#include <iostream>
#include <map>
#include <thread>

#define OFFSET 10
#define LINE_DETAILS 3.0f
#define PI 3.14159f
#define GMT_DIFF -4

int main()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	float horizontal = (float)desktop.right;
	float vertical = (float)desktop.bottom;
	float minimumWindow;
	if (horizontal > vertical)
		minimumWindow = vertical;
	else
		minimumWindow = horizontal;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(desktop.right, desktop.bottom), "Clock", sf::Style::Fullscreen, settings);

	float radius = (minimumWindow - OFFSET) / 2.0f;
	sf::CircleShape rim(radius, 100);
	rim.setOrigin(radius, radius);
	rim.setPosition(horizontal / 2, vertical / 2);
	rim.setFillColor(sf::Color(0, 0, 0));
	rim.setOutlineThickness(LINE_DETAILS);
	rim.setOutlineColor(sf::Color(255, 255, 255));

	sf::RectangleShape hour(sf::Vector2f(radius / 3.0f, LINE_DETAILS)),
		min(sf::Vector2f(radius * 2 / 3.0f, LINE_DETAILS)),
		sec(sf::Vector2f(radius * 3 / 4.0f, LINE_DETAILS));
	hour.setOrigin(0, LINE_DETAILS / 2);
	min.setOrigin(0, LINE_DETAILS / 2);
	sec.setOrigin(0, LINE_DETAILS / 2);
	hour.setPosition(horizontal / 2, vertical / 2);
	min.setPosition(horizontal / 2, vertical / 2);
	sec.setPosition(horizontal / 2, vertical / 2);

	std::map<int, sf::RectangleShape> markers;
	for (int rotation = 0; rotation < 360; rotation += 30) {
		markers[rotation] = sf::RectangleShape(sf::Vector2f(radius / 4, LINE_DETAILS));
		sf::RectangleShape& line = markers[rotation];
		line.rotate((float)rotation);
		line.setOrigin(0, LINE_DETAILS / 2);
		line.setPosition(horizontal / 2 - radius * cos(rotation * PI / 180), vertical / 2 - radius * sin(rotation * PI / 180));
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			return EXIT_SUCCESS;
		}
	}
	return EXIT_SUCCESS;
}
