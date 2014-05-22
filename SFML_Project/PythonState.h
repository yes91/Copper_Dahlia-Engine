#pragma once

#include "IGameState.h"

#include <boost\python.hpp>

struct PythonState : public IGameState, public boost::python::wrapper<IGameState>
{
public:
	int getID();
	virtual void init(StateBasedGame& game);
	virtual void handleEvents(sf::Event event, StateBasedGame& game);
	virtual void update(sf::RenderWindow& container, float dt, StateBasedGame& game);
	virtual void render(sf::RenderWindow& g, float alpha);
	virtual void enter(StateBasedGame& game);
	virtual void leave(StateBasedGame& game);
};