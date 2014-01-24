#pragma once

#include <SFML/System/Vector2.hpp>

struct MoveEvent
{
	sf::Vector2f move_vector;
	bool run;
};