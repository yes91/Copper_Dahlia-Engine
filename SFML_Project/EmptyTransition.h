#pragma once

#include "Transition.h"

class EmptyTransition : public Transition
{
public:
	void update(float dt);
	void preRender(sf::RenderTarget& g, StateBasedGame& game);
	void postRender(sf::RenderTarget& g, StateBasedGame& game);
	bool isComplete();
	void init(StatePtr first, StatePtr second);
};

