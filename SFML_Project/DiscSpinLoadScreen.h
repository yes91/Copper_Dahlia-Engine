#pragma once

#include "ILoadScreen.h"
#include "ResHandle.h"
#include <memory>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Sprite.hpp>

template<class T>
class ResHandle;

class StateBasedGame;

class DiscSpinLoadScreen : public ILoadScreen
{
	sf::VertexArray screen;
	sf::Text text;
	Resource<sf::Font>::type font;
	Resource<sf::Texture>::type spinner;
	sf::Sprite cd_sprite;

public:
	DiscSpinLoadScreen(StateBasedGame& game);
	void render(sf::RenderWindow& g);
	void update(float dt);
};

