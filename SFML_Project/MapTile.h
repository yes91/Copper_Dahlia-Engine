#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderStates.hpp>

namespace tmx
{
	//represents a single tile on a layer
	struct MapTile
	{
		//returns the base centre point of sprite / tile
		sf::Vector2f GetBase() const
		{
			return sf::Vector2f(sprite.getPosition().x + (sprite.getLocalBounds().width / 2.f),
				sprite.getPosition().y + sprite.getLocalBounds().height);
		}
		sf::Sprite sprite;
		sf::Vector2i gridCoord;
		sf::RenderStates renderStates; //used to perform any rendering with custom shaders or blend mode
	};
};