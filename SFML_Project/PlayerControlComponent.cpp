#include "PlayerControlComponent.h"
#include "Util.h"
#include <SFML/Window.hpp>
#include "Registrar.h"

static Registrar<PlayerControlComponent> registrar;

PlayerControlComponent::PlayerControlComponent(GameObject* owner, unsigned int controller)
	:MoveComponentBase(owner)
{
	cont = controller;
	paused = false;
}

bool PlayerControlComponent::isRun()
{
	return sf::Joystick::isButtonPressed(cont, 0) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
}

sf::Vector2f PlayerControlComponent::getMove()
{
	sf::Vector2f move_amount;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		move_amount.y -= 1.0f;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		move_amount.y += 1.0f;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		move_amount.x -= 1.0f;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		move_amount.x += 1.0f;
	}

	float deadZone = 0.2f;

	float x = sf::Joystick::getAxisPosition(cont, sf::Joystick::X)/100.0f;
	float y = sf::Joystick::getAxisPosition(cont, sf::Joystick::Y)/100.0f;

	move_amount.x += (std::abs(x) > deadZone) ? x:0.0f;
	move_amount.y += (std::abs(y) > deadZone) ? y:0.0f;

	if(util::length(move_amount) > 0.0f)
	{
		move_amount /= util::length(move_amount);
	}

	return move_amount;
}