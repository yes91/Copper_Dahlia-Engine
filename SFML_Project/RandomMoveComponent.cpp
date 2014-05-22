#include "RandomMoveComponent.h"
#include "Util.h"
#include "GameObject.h"
#include <ctime>
#include <random>

#define PI 3.14159265f

#include "Registrar.h"

Registrar<RandomMoveComponent> registrar;

sf::Vector2f RandomMoveComponent::getMove()
{

	sf::Vector2f move_amount;

	int i = rand.RandomRange(-1, 8);

	float angle = 0.0f;

	if(i <= 8 && i > 0){
		angle = i*(2*PI/8);
	}

	if(i > 0)
	{
		move_amount.x = std::cos(angle);
		move_amount.y = std::sin(angle);
	}
	else 
	{
		move_amount.x = 0.0f;
		move_amount.y = 0.0f;
	}

	if(util::length(move_amount) > 0.0f)
	{
		move_amount /= util::length(move_amount);
	}

	return move_amount;
}

void RandomMoveComponent::update(sf::RenderTarget& container, float dt)
{ 
	if(move_time <= 0.f)
	{
		move_time = getDuration();
		current_move = getMove();
	}

	move_time -= dt;

	MoveEvent e;
	e.move_vector = current_move;
	e.run = false;
	m_pOwner->triggerEvent(e);
}

float RandomMoveComponent::getDuration()
{
	return float(rand.RandomRange(100, 200)/100.f);
}