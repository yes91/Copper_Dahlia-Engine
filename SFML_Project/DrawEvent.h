#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

struct DrawEvent
{
	sf::RenderTarget* target;
	sf::RenderStates states;
};