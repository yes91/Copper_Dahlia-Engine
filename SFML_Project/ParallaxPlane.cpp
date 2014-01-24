#include "ParallaxPlane.h"

ParallaxPlane::ParallaxPlane(std::string file, sf::Vector2f size, sf::Vector2f speed)
{
	tex = MultiResCache::getInstance().GetHandle<sf::Texture>(file);
	tex->getWrite().setRepeated(true);

	glFlush();

	sprite.setTexture(tex->get());

	tex_size = static_cast<sf::Vector2f>(tex->get().getSize());

	sprite.scale(size.x/tex_size.x, size.y/tex_size.y);

	scroll_speed = speed;

	offset = sf::Vector2f(0.0f, 0.0f);
}

void ParallaxPlane::update(float dt)
{
	sf::FloatRect rect(offset.x, offset.y, tex_size.x, tex_size.y);
	sprite.setTextureRect(static_cast<sf::IntRect>(rect));

	offset += (scroll_speed * dt);
}

void ParallaxPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
}