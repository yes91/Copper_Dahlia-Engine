#pragma once

#include <SFML/Graphics.hpp>

struct WindowState
{
	sf::VideoMode mode;
	std::string title;
	sf::ContextSettings settings;
	bool fullscreen;
	bool vSync;
	bool cursor;
};

class IGame
{
public:
	virtual void handleEvents(sf::RenderWindow & container) = 0;
	virtual void update(sf::RenderWindow& container, float dt) = 0;
	virtual void render(sf::RenderWindow& g, float alpha) = 0;
	virtual void run() = 0;
};