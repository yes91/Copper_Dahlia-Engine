#pragma once

#include<SFML\Graphics\RenderWindow.hpp>

class ILoadScreen
{
	virtual void render(sf::RenderWindow& g) = 0;
	virtual void update(float dt) = 0;
public:
	void run(sf::RenderWindow& container, bool& done)
	{
		container.setActive(true);
		sf::Clock dtimer;

		const float fps = 120;
		const float dt = 1 / fps;
		float accumulator = 0;

		while (!done)
		{
			// Store the time elapsed since the last frame began
			const float currentTime = dtimer.restart().asSeconds();
			accumulator += currentTime;

			// Avoid spiral of death and clamp dt, thus clamping
			// how many times the update can be called in
			// a single game loop.
			if (accumulator > 0.2f)
			{
				accumulator = 0.2f;
			}

			while (accumulator > dt)
			{
				update(dt);
				accumulator -= dt;
			}


			container.clear();
			render(container);
			container.display();
		}
	}
};