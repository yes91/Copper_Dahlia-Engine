#pragma once

#include "ComponentBase.h"
#include "Util.h"
#include "MoveEvent.h"
#include "HeadingChangedEvent.h"
#include "GameObject.h"
#include "pugixml\pugixml.hpp"

class DirectionComponent : public ComponentBase<DirectionComponent>
{
public:

	enum Direction 
	{
		E = 0, NE = 1,
		N = 2, NW = 3,
		W = 4, SW = 5,
		S = 6, SE = 7
	};

private:

	static Direction fromString(std::string s)
	{
		if(s == "E") { return E; } 
		else if(s == "NE") { return NE; }
		else if(s == "N")  { return N; }
		else if(s == "NW")  { return NW; }
		else if(s == "W")  { return W; }
		else if(s == "SW")  { return SW; }
		else if(s == "S")  { return S; }
		else if(s == "SE")  { return SE; }

		return S;
	}

public:

	DirectionComponent(GameObject* owner, Direction d)
		:ComponentBase(owner), heading(d)
	{
		delegates.push_back(owner->addListener<MoveEvent>([this](MoveEvent e) { this->onMove(e);}));
	}

	static void create(GameObject* owner, const pugi::xml_node& node)
	{
		if(node.attribute("value"))
		{
			Direction d = fromString(node.attribute("value").as_string());
			owner->add<DirectionComponent>(d);
		}
	}

	void update(sf::RenderTarget& container, float dt) {}

	void onMove(MoveEvent e)
	{
		if(util::length(e.move_vector) > 0.f)
		{
			int octant = util::sector(e.move_vector, 8);

			if(octant != (int)heading)
			{
				setHeading(octant);
				HeadingChangedEvent ev;
				ev.heading = heading;
				m_pOwner->triggerEvent(ev);
			}
		}
	}

	void setHeading(int i)
	{
		i = std::abs(i);
		heading = (Direction) (i % 8);
	}

	Direction getHeading()
	{
		return heading;
	}

	bool equals (const DirectionComponent& rhs) const
	{
		return this->heading == rhs.heading;
	}

private:
	Direction heading;
};