#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>

#define PI 3.14159265f

namespace util {

	extern float length(const sf::Vector2f& vec);

	extern sf::Vector2f norm(const sf::Vector2f& vec);

	extern void position(sf::Vertex* quad, const sf::IntRect& rect);

	extern void texCoords(sf::Vertex* quad, const sf::IntRect& rect);

	extern void color(sf::Vertex* quad, const sf::Color& color);

	extern int sector(const sf::Vector2f& vec, int divisions);

	extern float scaleRange(float i, float min, float max, float newMin, float newMax);

}