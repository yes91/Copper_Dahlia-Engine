#pragma once

#include "Process.h"
#include <iostream>
#include <SFML/System/Time.hpp>

#include "GameObject.h"

class PlayerColorRandomizer : public CloneableProcess<PlayerColorRandomizer>
{
	sf::Time delay;
	sf::Time accum;

	std::weak_ptr<GameObject> o;
public:
	PlayerColorRandomizer(std::shared_ptr<GameObject> o )
		:CloneableProcess()
	{ 
		this->o = std::weak_ptr<GameObject>(o); delay = sf::seconds(1.f); 
	}

protected:
	virtual void update(float dt)
	{
		if(o.expired())
		{
			fail();
		}

		auto go = o.lock();
		accum += sf::seconds(dt);
		if(go->has<ActorSpriteComponent>())
		{
			go->get<ActorSpriteComponent>().setColor(sf::Color(rand()*255, rand()*255, rand()*255));
		}

		if(accum.asSeconds() > delay.asSeconds())
		{
			succeed();
		}
	}
};