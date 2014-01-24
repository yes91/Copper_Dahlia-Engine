#pragma once

#include "Process.h"
#include <iostream>
#include <SFML/System/Time.hpp>

class DelayProcess : public CloneableProcess<DelayProcess>
{
	sf::Time delay;
	sf::Time accum;
public:
	DelayProcess(sf::Time delay)
	{
		this->delay = delay;
	}

protected:
	virtual void update(float dt)
	{
		accum += sf::seconds(dt);
		if(accum.asSeconds() > delay.asSeconds())
		{
			succeed();
		}
	}
};