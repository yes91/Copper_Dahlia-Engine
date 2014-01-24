#pragma once

namespace sf 
{
	class RenderWindow;
};

class ILoadScreen
{
public:
	virtual void render(sf::RenderWindow& g) = 0;
	virtual void update(float dt) = 0;
};