#pragma once
#include "igamestate.h"

class EmptyState :	public IGameState
{
public:
	int getID();
	void init(StateBasedGame& game);
	void handleEvents(sf::Event event, StateBasedGame& game);
	void update(sf::RenderWindow& container, float dt, StateBasedGame& game);
	void render(sf::RenderWindow& g, float alpha);
	void enter(StateBasedGame& game);
	void leave(StateBasedGame& game);
};

