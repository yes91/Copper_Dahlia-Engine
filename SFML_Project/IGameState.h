#pragma once

#include <SFML/Graphics.hpp>
#include "EventDispatcher.h"
#include "ProcessManager.h"

class StateBasedGame;

class IGameState
{
protected:
	ProcessManager process_man;
	EventDispatcher dispatch;
public:
	typedef std::shared_ptr<IGameState> ptr;

	IGameState() { paused = false; initialized = false;}
	virtual ~IGameState() {}
	virtual int getID() = 0;
	virtual void init(StateBasedGame& game) = 0;
	virtual void handleEvents(sf::Event event, StateBasedGame& game) = 0;
	virtual void update(sf::RenderWindow& container, float dt, StateBasedGame& game) = 0;
	virtual void render(sf::RenderWindow& g, float alpha) = 0;
	virtual void enter(StateBasedGame& game) = 0;
	virtual void leave(StateBasedGame& game) = 0;
	virtual bool isPaused() { return paused; }
	virtual bool isInitialized() { return initialized; }
	
	virtual EventDispatcher& getDisptach() { return dispatch; };
	virtual ProcessManager& getProcessor() { return process_man; };

	bool paused;
	bool initialized;
};