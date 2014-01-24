#include "Util.h"
#include <cmath>
#include <iostream>

namespace util 
{

	float length(const sf::Vector2f& vec)
	{
		return std::sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	sf::Vector2f norm(const sf::Vector2f& vec)
	{
		sf::Vector2f vector(vec);
		if(length(vector) > 0.0f)
			vector /= length(vector);
		return vector;
	}

	void position(sf::Vertex* quad, const sf::IntRect& rect)
	{
		sf::FloatRect rectangle = static_cast<sf::FloatRect>(rect);
		quad[0].position = sf::Vector2f(rectangle.left, rectangle.top);
		quad[1].position = sf::Vector2f(rectangle.left + rectangle.width, rectangle.top);
		quad[2].position = sf::Vector2f(rectangle.left + rectangle.width, rectangle.top + rectangle.height);
		quad[3].position = sf::Vector2f(rectangle.left, rectangle.top + rectangle.height);
	}

	void texCoords(sf::Vertex* quad, const sf::IntRect& rect)
	{
		sf::FloatRect rectangle = static_cast<sf::FloatRect>(rect);
		quad[0].texCoords = sf::Vector2f(rectangle.left, rectangle.top);
		quad[1].texCoords = sf::Vector2f(rectangle.left + rectangle.width, rectangle.top);
		quad[2].texCoords = sf::Vector2f(rectangle.left + rectangle.width, rectangle.top + rectangle.height);
		quad[3].texCoords = sf::Vector2f(rectangle.left, rectangle.top + rectangle.height);
	}

	void color(sf::Vertex* quad, const sf::Color& col)
	{
		for(int i = 0; i < 4; i++)
		{
			quad[i].color = col;
		}
	}

	int sector(const sf::Vector2f& vec, int divisions)
	{
		float angle = -std::atan2( vec.y, vec.x );
		if(angle < 0.0f) angle += 2*PI;
		int result = int( divisions * angle / (2*PI) + (divisions + 0.5f) ) % divisions;
		return result;
	}

	float scaleRange(float i, float min, float max, float newMin, float newMax) 
	{
		return (((newMax-newMin)*(i - min))/(max - min)) + newMin;
	}
}