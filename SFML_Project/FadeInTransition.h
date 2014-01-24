#pragma once
#include "transition.h"
#include <SFML\System\Time.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

class FadeInTransition : public Transition
{
public:
	FadeInTransition();
	FadeInTransition(sf::Color color);
	FadeInTransition(sf::Color color, sf::Time duration);
	void update(float dt);
	void preRender(sf::RenderTarget& g, StateBasedGame& game);
	void postRender(sf::RenderTarget& g, StateBasedGame& game);
	bool isComplete();
	void init(StatePtr first, StatePtr second);
private:
	sf::Color color;
	float alpha;
	sf::RectangleShape rect;
	sf::Time duration;
};

