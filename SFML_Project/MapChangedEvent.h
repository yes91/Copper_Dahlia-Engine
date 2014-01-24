#pragma once

#include <SFML/System/Vector2.hpp>

struct MapChangedEvent
{
	sf::Vector2f bounds;
};