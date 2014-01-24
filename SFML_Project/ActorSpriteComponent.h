#pragma once

#include "AnimatedSprite.h"
#include "MultiResCache.h"
#include "GameObject.h"
#include "DirectionComponent.h"
#include "SizeChangedEvent.h"
#include "MoveEvent.h"
#include "DrawEvent.h"
#include "HeadingChangedEvent.h"
#include <iostream>
#include <sstream>

class ActorSpriteComponent : public ComponentBase<ActorSpriteComponent>
{
public:

	ActorSpriteComponent(GameObject* owner, std::string img, bool eight_dir)
		:ComponentBase(owner)
	{
		image = MultiResCache::getInstance().GetHandle<sf::Texture>(img);

		sprite = AnimatedSprite(sf::seconds(0.1f));

		int width = image->get().getSize().x;
		int height = image->get().getSize().y;

		int frames_x = 4;
		int frames_y = (eight_dir) ? 8:4;

		animations = std::vector<Animation>(frames_y);

		for(int i = 0; i < frames_y; i++)
		{
			animations[i].setSpriteSheet(image->get());
			for(int j = 0; j < frames_x; j++)
			{
				animations[i].addFrame(sf::IntRect((width/frames_x)*j, i*(height/frames_y), (width/frames_x), (height/frames_y)));
			}
		}

		sprite.setAnimation(animations[0]);

		delegates.push_back(owner->addListener<DrawEvent>([this](DrawEvent e){ this->onDraw(e);}));
		delegates.push_back(owner->addListener<MoveEvent>([this](MoveEvent e){ this->onMove(e);}));
		delegates.push_back(owner->addListener<HeadingChangedEvent>([this](HeadingChangedEvent e){ this->onChangeDirection(e);}));

		float S_width = this->getLocalBounds().width;
		float S_height = this->getLocalBounds().height;

		SizeChangedEvent e;
		e.size = sf::Vector2f(S_width, S_height);
		owner->triggerEvent(e);

		owner->setOrigin(S_width/2.f, S_height/2.f);
	}

	static void create(GameObject* owner, const pugi::xml_node& node)
	{
		if(node.attribute("value"))
		{
			std::cout << "Processing ActorSpriteComponent arguments..." << std::endl;
			std::string values = node.attribute("value").as_string();
			std::stringstream datastream(values);
			
			//parse csv string into vector of strings
			std::vector<std::string> args;
			std::string arg;
			while (std::getline(datastream, arg, ','))
			{
				args.push_back(arg);
				if(datastream.peek() == ',')
					datastream.ignore();
			}

			pugi::xml_attribute two;
			two.set_value(args[1].c_str());

			std::cout << args[0].c_str() << ":" << two.as_bool() << std::endl;

			owner->add<ActorSpriteComponent>(args[0].c_str(), two.as_bool());
		}
	}

	void setColor(sf::Color col)
	{
		sprite.setColor(col);
	}

	sf::FloatRect getLocalBounds()
	{
		return sprite.getLocalBounds();
	}

	void onMove(MoveEvent e)
	{
		(util::length(e.move_vector) > 0.f) ? sprite.play():sprite.stop();
	}

	void onChangeDirection(HeadingChangedEvent e)
	{
		unsigned int index = 0;

		switch(e.heading)
		{
		case DirectionComponent::N: index = 3; break;
		case DirectionComponent::S: index = 0; break;
		case DirectionComponent::W: index = 1; break;
		case DirectionComponent::E: index = 2; break;
		case DirectionComponent::SW: index = 4; break;
		case DirectionComponent::SE: index = 5; break;
		case DirectionComponent::NW: index = 6; break;
		case DirectionComponent::NE: index = 7; break;
		}

		if(index < animations.size())
		{
			if(sprite.getAnimation() != &animations[index])
				sprite.setAnimation(animations[index]);
		}
	}

	void update(sf::RenderTarget& container, float dt)
	{
		sprite.update(dt);
	}

	void onDraw(DrawEvent e)
	{
		e.target->draw(sprite, e.states);
	}

private:

	AnimatedSprite sprite;
	std::vector<Animation> animations;
	shared_ptr<ResHandle<sf::Texture>> image;
};