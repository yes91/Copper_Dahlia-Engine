#pragma once

class GameObject;
#include <SFML/Graphics/Rect.hpp>

struct CollisionEvent
{
	sf::Vector2f left_top;
	sf::FloatRect intersection;
};