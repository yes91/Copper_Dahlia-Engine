#include "FadeInTransition.h"

#include "StateBasedGame.h"

FadeInTransition::FadeInTransition()
{
	this->color = sf::Color::Black;
	this->color.a = 255;
	alpha = 1.f;
	this->duration = sf::seconds(0.5f);
}

FadeInTransition::FadeInTransition(sf::Color color)
{
	this->color = color;
	this->color.a = 255;
	alpha = 1.f;
	this->duration = sf::seconds(0.5f);
}

FadeInTransition::FadeInTransition(sf::Color color, sf::Time duration)
{
	this->color = color;
	this->color.a = 255;
	alpha = 1.f;
	this->duration = duration;
}

void FadeInTransition::update(float dt)
{
	alpha -= dt * (1.0f/duration.asSeconds());
	color.a = int(alpha * 255.f);
	if (alpha < 0.f) 
	{
		color.a = 0;
	}
}

void FadeInTransition::preRender(sf::RenderTarget& g, StateBasedGame& game)
{

}

void FadeInTransition::postRender(sf::RenderTarget& g, StateBasedGame& game)
{
	g.setView(g.getDefaultView());
	rect.setSize(static_cast<sf::Vector2f>(g.getSize()));
	rect.setFillColor(color);
	g.draw(rect);
}

bool FadeInTransition::isComplete()
{
	return (alpha <= 0.f);
}

void FadeInTransition::init(StatePtr first, StatePtr second)
{

}

