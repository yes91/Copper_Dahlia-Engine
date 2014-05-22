#pragma once

#include "MoveComponentBase.h"
#include "MoveEvent.h"
#include "GameObject.h"
#include "pugixml\pugixml.hpp"
#include <iostream>

class PlayerControlComponent : public MoveComponentBase<PlayerControlComponent>
{
public:

	PlayerControlComponent(GameObject* owner, unsigned int controller);

	~PlayerControlComponent(){ std::cout << "PCC died!" << std::endl; }

	static void create(GameObject* owner, const pugi::xml_node& node)
	{
		if(node.attribute("value"))
		{
			int i = node.attribute("value").as_int();
			owner->add<PlayerControlComponent>(i);
		}
	}

	void update(sf::RenderTarget& container, float dt) 
	{ 
		MoveEvent e;
		if(!paused)
		{
			e.move_vector = getMove();
			e.run = isRun();
		}
		m_pOwner->triggerEvent(e); 
	}

	void suspend()
	{
		paused = true;
	}

	void resume()
	{
		paused = false;
	}

	bool isPaused()
	{
		return paused;
	}

	sf::Vector2f getMove();

	bool isRun();

	unsigned int cont;

	bool equals (const PlayerControlComponent& rhs) const
	{
		return this->cont == rhs.cont;
	}

private:
	bool paused;
};