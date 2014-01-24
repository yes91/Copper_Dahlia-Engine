#include "FadeOutTransition.h"

#include "StateBasedGame.h"

#include <iostream>

FadeOutTransition::FadeOutTransition()
{
	this->color = sf::Color::Black;
	this->color.a = 0;
	alpha = 0.f;
	this->duration = sf::seconds(0.5f);
}

FadeOutTransition::FadeOutTransition(sf::Color color)
{
	this->color = color;
	this->color.a = 0;
	alpha = 0.f;
	this->duration = sf::seconds(0.5f);
}

FadeOutTransition::FadeOutTransition(sf::Color color, sf::Time duration)
{
	this->color = color;
	this->color.a = 0;
	alpha = 0.f;
	this->duration = duration;
}

void FadeOutTransition::update(float dt)
{
	alpha += dt * (1.0f/duration.asSeconds());
	color.a = int(alpha * 255.f);
	if (alpha > 1.0f) 
	{
		color.a = 255;
	}
}

void FadeOutTransition::preRender(sf::RenderTarget& g, StateBasedGame& game)
{

}

void FadeOutTransition::postRender(sf::RenderTarget& g, StateBasedGame& game)
{
	g.setView(g.getDefaultView());
	rect.setSize(static_cast<sf::Vector2f>(g.getSize()));
	rect.setFillColor(color);
	g.draw(rect);
}

bool FadeOutTransition::isComplete()
{
	return (alpha >= 1.0f);
}

void FadeOutTransition::init(StatePtr first, StatePtr second)
{

}
