#include "PythonState.h"

#include "StateBasedGame.h"

int PythonState::getID()
{
	return this->get_override("get_id")();
}

void PythonState::init(StateBasedGame& game)
{
	this->get_override("init")(boost::ref(game));
}

void PythonState::handleEvents(sf::Event event, StateBasedGame& game)
{
	this->get_override("handle_events")(event, boost::ref(game));
}

void PythonState::update(sf::RenderWindow& container, float dt, StateBasedGame& game)
{
	this->get_override("update")(boost::ref(container), dt, boost::ref(game));
}

void PythonState::render(sf::RenderWindow& g, float alpha)
{
	this->get_override("render")(boost::ref(g), alpha);
}

void PythonState::enter(StateBasedGame& game)
{
	this->get_override("enter")(boost::ref(game));
}

void PythonState::leave(StateBasedGame& game)
{
	this->get_override("leave")(boost::ref(game));
}