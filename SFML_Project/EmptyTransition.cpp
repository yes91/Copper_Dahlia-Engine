#include "EmptyTransition.h"

#include "StateBasedGame.h"

void EmptyTransition::update(float dt)
{

}

void EmptyTransition::preRender(sf::RenderTarget& g, StateBasedGame& game)
{

}

void EmptyTransition::postRender(sf::RenderTarget& g, StateBasedGame& game)
{

}

bool EmptyTransition::isComplete()
{
	return true;
}

void EmptyTransition::init( StatePtr first, StatePtr second )
{

}

