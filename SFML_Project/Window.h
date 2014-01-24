#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include "WindowSkinRes.h"
#include "EngineStd.h"
#include "EventListener.h"
#include "StateBasedGame.h"
#include "MultiResCache.h"

class Window : public sf::Transformable, public EventListener
{
public:
	Window(int width, int height, EventDispatcher& disp);

	void setOpacity(float percent);

	void resize(int width, int height);

	sf::Vector2u getSize() { return size; }

protected:

	static const int WINDOW_LINE_HEIGHT = 24;
	int oy;

	int getBorderWidth() { return m_border_width; }

	sf::RenderTexture contents;
	sf::Vector2u size;
	shared_ptr<ResHandle<WindowSkinRes>> m_skin;

public:
	void draw(sf::RenderTarget& target, sf::RenderStates states);

private:

	int m_border_width;

	//Window
	sf::VertexArray m_border;
	sf::VertexArray m_background;
	sf::VertexArray m_pattern;
	sf::VertexArray m_border_top;
	sf::VertexArray m_border_right;
	sf::VertexArray m_border_left;
	sf::VertexArray m_border_bottom;
};