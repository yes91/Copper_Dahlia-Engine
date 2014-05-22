#pragma once

#include "EventProcess.h"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>
#include "DrawEvent.h"
#include "MultiResCache.h"

class MessageProcess : public EventProcess<MessageProcess>
{
	sf::Time delay;
	sf::Time accum;
	float type_delay;
	Resource<sf::Font>::type font;
	sf::Text text;
	std::string message;
	unsigned int i;
public:
	MessageProcess(EventDispatcher& disp, sf::Time duration)
		:EventProcess(disp)
	{
		this->delay = duration;
		font = MultiResCache::getInstance().GetHandle<sf::Font>("sansation.ttf");
		text.setFont(font->get());
		message = "This is a message";
		i = 0;
		type_delay = 0.0f;
	}

	~MessageProcess()
	{
		dispatch.removeListener<DrawEvent>(fastdelegate::MakeDelegate(this, &MessageProcess::onDraw));
	}

	MessageProcess* clone() 
	{ 
		auto c = new MessageProcess(*this); 
		c->m_pChild = std::shared_ptr<Process>(m_pChild->clone());
		return c; 
	}

protected:

	virtual void init()
	{
		EventProcess::init();
		dispatch.addListener<DrawEvent>(fastdelegate::MakeDelegate(this, &MessageProcess::onDraw));
	}

	virtual void update(float dt)
	{
		accum += sf::seconds(dt);
		type_delay += dt;
		text.move(5.f * dt, 0.f);
		if(type_delay > 0.05f && i < message.length())
		{
			text.setString(text.getString() + message[i]);
			i++;
			type_delay = 0.0f;
		}
		if(accum.asSeconds() > delay.asSeconds())
		{
			succeed();
		}
	}

	void onDraw(DrawEvent e)
	{
		e.target->draw(text);
	}
};