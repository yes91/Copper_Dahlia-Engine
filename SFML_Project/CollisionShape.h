#pragma once

#include <SFML\Graphics\Transformable.hpp>
#include <vector>

namespace sf
{
	class CollisionShape : public Transformable
	{
	public:
		void addPoint(const sf::Vector2f& point);
	private:
		struct Edge
		{
			Edge(const sf::Vector2f& start, const sf::Vector2f& end) 
			{
				this->start = start; 
				this->end = end;
			}

			bool intersects(const Edge& other);

			sf::Vector2f start;
			sf::Vector2f end;
		};

		std::vector<sf::Vector2f> m_polyPoints;
		std::vector<Edge> m_polyEdges;
	};
};