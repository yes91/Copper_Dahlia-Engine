#pragma once

#include "PhysicsComponentBase.h"
#include <SFML/System/Vector2.hpp>
#include "SizeChangedEvent.h"
#include "MapLoader.h"
#include "MoveEvent.h"
#include "CollisionEvent.h"
#include "MapChangedEvent.h"
#include "GameObject.h"
#include <iostream>
#include <sstream>

#include "MapObjectComponent.h"

class ActorPhysicsComponent : public PhysicsComponentBase<ActorPhysicsComponent>
{
public:
	ActorPhysicsComponent(GameObject* owner, sf::Vector2f v_init, float v_max, float t_max)
		:PhysicsComponentBase(owner)
	{ 
		velocity = v_init;
		friction = 5.0f/t_max;
		acceleration = v_max * friction;
		paused = false;

		owner->addListener<MoveEvent>(fastdelegate::MakeDelegate(this, &ActorPhysicsComponent::onMove));

		owner->addListener<SizeChangedEvent>(fastdelegate::MakeDelegate(this, &ActorPhysicsComponent::onSizeChanged));

		owner->addListener<CollisionEvent>(fastdelegate::MakeDelegate(this, &ActorPhysicsComponent::onCollision));

	}

	void registerEvents(EventDispatcher& dispatch)
	{
		m_pOwner->addGlobalListener<MapChangedEvent>(dispatch, fastdelegate::MakeDelegate(this, &ActorPhysicsComponent::onMapChanged));
	}

	void unregisterEvents(EventDispatcher& dispatch)
	{
		m_pOwner->removeGlobalListener<MapChangedEvent>(dispatch, fastdelegate::MakeDelegate(this, &ActorPhysicsComponent::onMapChanged));
	}

	static void create(GameObject* owner, const pugi::xml_node& node)
	{
		if(node.attribute("value"))
		{
			std::string values = node.attribute("value").as_string();
			std::stringstream datastream(values);
			
			//parse csv string into vector of floats
			std::vector<float> args;
			float arg;
			while (datastream >> arg)
			{	
				args.push_back(arg);
				if(datastream.peek() == ',')
					datastream.ignore();
			}

			owner->add<ActorPhysicsComponent>(sf::Vector2f(args[0], args[1]), args[2], args[3]);
		}
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

	void update(sf::RenderTarget& container, float dt) 
	{
		if(!paused)
		{
			m_pOwner->move(velocity * dt);

			bounds_check();

			sf::Vector2f accel = (move_amount * (acceleration * (run ? 2.5f:1.0f))) + (friction * -velocity);
			velocity += accel * dt;	
		}
	}

	void onCollision(CollisionEvent e)
	{
		if (e.intersection.width < e.intersection.height) {
			// collided on X
			if (this->getGlobalBounds().left < e.left_top.x ) {
				m_pOwner->move( -e.intersection.width , 0.f);
			}else {
				m_pOwner->move( e.intersection.width, 0.f);
			}
		}

		if (e.intersection.width > e.intersection.height) {
			// collided on Y
			if (this->getGlobalBounds().top < e.left_top.y ) {
				m_pOwner->move( 0.f , -e.intersection.height);
			}else {
				m_pOwner->move( 0.f, e.intersection.height);
			}
		}

		bounds_check();
	}

	void onMapChanged(MapChangedEvent e)
	{
		global_bounds = e.bounds;
	}

	void onSizeChanged(SizeChangedEvent e)
	{
		size = e.size;
	}

	void onMove(MoveEvent e)
	{
		move_amount = e.move_vector;
		run = e.run;
	}

	sf::FloatRect getGlobalBounds()
	{
		float width = size.x;
		float height = size.y;
		return m_pOwner->getTransform().transformRect(sf::FloatRect(width/4.f, height/2.f, width - width/2.f, height/2.f));
	}

	sf::Vector2f size;
	sf::Vector2f global_bounds;
	sf::Vector2f move_amount;
	bool run;
	sf::Vector2f velocity;
	float friction;
	float acceleration;

	bool equals (const ActorPhysicsComponent& rhs) const
	{
		return this->velocity == rhs.velocity && this->friction == rhs.friction && this->acceleration == rhs.acceleration;
	}

private:

	void bounds_check()
	{
		float x = m_pOwner->getPosition().x;

		float y =  m_pOwner->getPosition().y;


		if((x + size.x/2.f) >=  global_bounds.x)
		{
			m_pOwner->move(global_bounds.x - (x + size.x/2.f), 0.0f);
		}
		else if(x - size.x/2.f <= 0.f)
		{
			m_pOwner->move(-(x - size.x/2.f), 0.0f);
		}
		if((y + size.y/2.f) >= global_bounds.y)
		{
			m_pOwner->move(0.0f, global_bounds.y - (y + size.y/2.f));
		}
		else if(y - size.y/2.f <= 0.f)
		{
			m_pOwner->move(0.0f, -(y - size.y/2.f));
		}
	}

	bool paused;
};