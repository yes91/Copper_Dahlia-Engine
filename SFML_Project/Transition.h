#pragma once
#include <SFML\Graphics\RenderTarget.hpp>
#include <memory>

class StateBasedGame;
class IGameState;

class Transition
{
protected:
	typedef std::shared_ptr<IGameState> StatePtr;
public:
	virtual void update(float dt) = 0;
	virtual void preRender(sf::RenderTarget& g, StateBasedGame& game) = 0;
	virtual void postRender(sf::RenderTarget& g, StateBasedGame& game) = 0;
	virtual bool isComplete() = 0;
	virtual void init(StatePtr first, StatePtr second) = 0;
};