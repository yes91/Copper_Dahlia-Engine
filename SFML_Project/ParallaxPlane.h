#pragma once
#include "EngineStd.h"
#include <SFML\Graphics.hpp>
#include "MultiResCache.h"

class ParallaxPlane : public sf::Drawable
{
public:
	ParallaxPlane(std::string file, sf::Vector2f size, sf::Vector2f speed);

	sf::Vector2f Scroll_speed() const { return scroll_speed; }
	void Scroll_speed(sf::Vector2f val) { scroll_speed = val; }

	void update(float dt);

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	shared_ptr<ResHandle<sf::Texture>> tex;
	sf::Vector2f tex_size;
	sf::Sprite sprite;
	sf::Vector2f scroll_speed;
	sf::Vector2f offset;
};