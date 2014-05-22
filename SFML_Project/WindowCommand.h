#pragma once

#include "WindowSelectable.h"

#include "KeyPressedEvent.h"

class WindowCommand : public WindowSelectable
{
public:
	std::vector<std::string> commands;
	std::vector<std::function<void()>> actions;

	Resource<sf::Font>::type font;
	sf::Text text;

	WindowCommand(std::vector<std::string> commands, int width, int cMax, int rMax, EventDispatcher& disp)
		:WindowSelectable(width, (rMax == 0 ? ((commands.size() + cMax - 1) / cMax):rMax) * WINDOW_LINE_HEIGHT + 32, disp)
	{
		font = MultiResCache::getInstance().GetHandle<sf::Font>("sansation.ttf");

		disp.addListener<KeyPressedEvent>(fastdelegate::MakeDelegate(this, &WindowCommand::onKeyPressed));

		text.setCharacterSize(18);
		text.setFont(font->get());

		this->commands = commands;
		ItemMax(commands.size());
		actions.resize(commands.size());
        ColumnMax(cMax);

		onRepaint();
	}

	void onRepaint()
	{
		contents.clear(sf::Color::Transparent);

		int i = 0;
		for(std::string s : commands)
		{
			text.setString(s);
			sf::IntRect rect = getItemRect(i);
			text.setPosition(static_cast<float>(rect.left + 4), static_cast<float>(rect.top));
			contents.draw(text, sf::BlendNone);
			i++;
		}

		contents.display();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states)
	{
		WindowSelectable::draw(target, states);

		states.transform *= getTransform();

		sf::Sprite sprite(contents.getTexture());
		sprite.move(static_cast<float>(getBorderWidth()/2), static_cast<float>(getBorderWidth()/2));
		target.draw(sprite, states);
	}

	void setAction(int id, std::function<void()> action)
	{
		if(id >= 0 && id <= ItemMax())
		{
			actions[id] = std::move(action);
		}
	}

	void onKeyPressed(KeyPressedEvent e)
	{
		WindowSelectable::onKeyPressed(e);
		if(e.code == sf::Keyboard::J)
		{
			if(Index() >= 0 && Index() <= ItemMax())
			{
				if(!actions[Index()]._Empty())
				{
					cursor_select.play();
					actions[Index()]();
				}
			}
		}
	}

};