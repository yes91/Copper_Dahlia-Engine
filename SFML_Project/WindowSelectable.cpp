#include "WindowSelectable.h"
#include "Util.h"
#include "MultiResCache.h"
#include "KeyPressedEvent.h"
#include <SFML/Window/Keyboard.hpp>

using namespace util;

WindowSelectable::WindowSelectable(int width, int height, EventDispatcher& disp)
	:Window(width, height, disp)
{
	itemMax = 1;
	columnMax = 1;
	index = -1;
	spacing = 16;

	time = 0.f;

	m_focused = false;

	cursor_sfx = MultiResCache::getInstance().GetHandle<sf::SoundBuffer>("res/Audio/SE/Cursor2.ogg");
	cursor.setBuffer(cursor_sfx->get());

	cursor_select_sfx = MultiResCache::getInstance().GetHandle<sf::SoundBuffer>("res/Audio/SE/Decision1.ogg");
	cursor_select.setBuffer(cursor_select_sfx->get());

	m_arrow_up.setPrimitiveType(sf::Quads);
	m_arrow_up.resize(1 * 4);
	m_arrow_down.setPrimitiveType(sf::Quads);
	m_arrow_down.resize(1 * 4);
	m_arrow_left.setPrimitiveType(sf::Quads);
	m_arrow_left.resize(1 * 4);
	m_arrow_right.setPrimitiveType(sf::Quads);
	m_arrow_right.resize(1 * 4);

	m_cursor.setPrimitiveType(sf::Quads);
	m_cursor.resize(9 * 4);

	int s_width =  m_skin->get().width;
	int s_height = m_skin->get().height;

	int arrow_width = s_width/8 - s_width/64;
	int arrow_height = s_height/16;

	contents.create(width - getBorderWidth(), height - getBorderWidth());
	contents.clear(sf::Color::Transparent);

	sf::Vertex* quad = &m_arrow_up[0];
	position(quad, sf::IntRect(width/2 - arrow_width/2, s_height/32, arrow_width, arrow_height));
	texCoords(quad, sf::IntRect(s_width/2 + s_width/8 + s_width/16 + s_width/128, s_height/8, arrow_width, arrow_height));

	quad = &m_arrow_down[0];
	position(quad, sf::IntRect(width/2 - arrow_width/2, height - arrow_height - s_height/32, arrow_width, arrow_height));
	texCoords(quad, sf::IntRect(s_width/2 + s_width/8 + s_width/16 + s_width/128, s_height/4 + arrow_height, arrow_width, arrow_height));

	quad = &m_arrow_left[0];
	position(quad, sf::IntRect(s_width/32, height/2 - arrow_width/2, arrow_height, arrow_width));
	texCoords(quad, sf::IntRect(s_width/2 + s_width/8, s_width/8 + s_width/16 + s_width/128, arrow_height, arrow_width));

	quad = &m_arrow_right[0];
	position(quad, sf::IntRect(width - arrow_height - s_width/32, height/2 - arrow_width/2, arrow_height, arrow_width));
	texCoords(quad, sf::IntRect(s_width/2 + s_width/4 + s_width/16, s_width/8 + s_width/16 + s_width/128, arrow_height, arrow_width));

	//Cursor
	sf::RectangleShape rect(sf::Vector2f(s_width/64, s_height/64));
	sf::RectangleShape rect2(sf::Vector2f(s_width/4 - s_width/32, s_height/64));

	rect.setPosition(s_width/2 + s_width/8, s_height/2 + s_height/8);
	rect2.setPosition(s_width/2 + s_width/64 + s_width/8 - s_width/64, s_height/2 + s_height/8);

	rect.setOrigin(s_width/8, s_height/8);
	rect2.setOrigin(s_width/8 - s_width/64, s_height/8);

	for(int i = 0; i < 8; i += 2)
	{
		rect.setRotation((i/2) * 90);
		sf::IntRect t_r_1 = (sf::IntRect)rect.getGlobalBounds();

		rect2.setRotation((i/2) * 90);
		sf::IntRect t_r_2 = (sf::IntRect)rect2.getGlobalBounds();

		quad = &m_cursor[i * 4];
		texCoords(quad, t_r_1);

		quad = &m_cursor[(i + 1) * 4];
		texCoords(quad, t_r_2);
	}
	
	quad = &m_cursor[8 * 4];
	texCoords(quad, sf::IntRect(s_width/2 + s_width/64, s_height/2 + s_height/64, s_width/4 - s_width/32, s_height/4 - s_height/32));
}

void WindowSelectable::setCursorRect(sf::IntRect cursorRect)
{
	int s_width = m_skin->get().width;
	int s_height = m_skin->get().height;

	sf::Vertex* quad = &m_cursor[0 * 4];
	position(quad, sf::IntRect(cursorRect.left, cursorRect.top, s_width/64, s_height/64));

	quad = &m_cursor[1 * 4];
	position(quad, sf::IntRect(cursorRect.left + s_width/64, cursorRect.top, cursorRect.width - s_width/32, s_height/64));

	quad = &m_cursor[2 * 4];
	position(quad, sf::IntRect(cursorRect.left + cursorRect.width - s_width/64, cursorRect.top, s_width/64, s_height/64));

	quad = &m_cursor[3 * 4];
	position(quad, sf::IntRect(cursorRect.left + cursorRect.width - s_width/64 , cursorRect.top + s_width/64, s_width/64, cursorRect.height - s_height/32));

	quad = &m_cursor[4 * 4];
	position(quad, sf::IntRect(cursorRect.left + cursorRect.width - s_width/64, cursorRect.top + cursorRect.height - s_width/64, s_width/64, s_height/64));

	quad = &m_cursor[5 * 4];
	position(quad, sf::IntRect(cursorRect.left + s_width/64, cursorRect.top + cursorRect.height - s_height/64, cursorRect.width - s_width/32, s_height/64));

	quad = &m_cursor[6 * 4];
	position(quad, sf::IntRect(cursorRect.left, cursorRect.top + cursorRect.height - s_height/64, s_width/64, s_height/64));

	quad = &m_cursor[7 * 4];
	position(quad, sf::IntRect(cursorRect.left, cursorRect.top + s_height/64, s_height/64, cursorRect.height - s_height/32));

	quad = &m_cursor[8 * 4];
	position(quad, sf::IntRect(cursorRect.left + s_width/64, cursorRect.top + s_height/64, cursorRect.width - s_height/32, cursorRect.height - s_height/32));
}

void WindowSelectable::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	Window::draw(target, states);

	states.transform *= getTransform();

	for(int i = 0; i < 9; i++)
	{
		sf::Vertex* quad = &m_cursor[i * 4];
		util::color(quad, sf::Color(255, 255, 255, int(util::scaleRange(std::sin(time*4.f), -1.f, 1.f, 150.f, 255.f))));
	}

	states.texture = &m_skin->get().m_main;
	states.transform.translate(getBorderWidth()/2, getBorderWidth()/2);
	if(index >= 0)
		target.draw(m_cursor, states);
	states.transform.translate(-getBorderWidth()/2, -getBorderWidth()/2);

	//target.draw(m_arrow_left, states);
	//target.draw(m_arrow_right, states);

	if(itemMax > getPageItemMax() && itemMax > 0)
	{
		states.texture = &m_skin->get().m_main;
		if(getBottomRow() < getRowMax() - 1)
		{
			target.draw(m_arrow_down, states);
		}
		if(getTopRow() > 0)
		{
			target.draw(m_arrow_up, states);
		}
	}
	
}

void WindowSelectable::resize(int width, int height)
{
	Window::resize(width, height);

	int s_width =  m_skin->get().width;
	int s_height = m_skin->get().height;

	int arrow_width = s_width/8 - s_width/64;
	int arrow_height = s_height/16;

	sf::Vertex* quad = &m_arrow_up[0];
	position(quad, sf::IntRect(width/2 - arrow_width/2, s_width/32, arrow_width, arrow_height));
	texCoords(quad, sf::IntRect(s_width/2 + s_width/8 + s_width/16 + s_width/128, s_height/8, arrow_width, arrow_height));

	quad = &m_arrow_down[0];
	position(quad, sf::IntRect(width/2 - arrow_width/2, height - arrow_height - s_height/32, arrow_width, arrow_height));
	texCoords(quad, sf::IntRect(s_width/2 + s_width/8 + s_width/16 + s_width/128, s_height/4 + arrow_height, arrow_width, arrow_height));

	quad = &m_arrow_left[0];
	position(quad, sf::IntRect(s_width/32, height/2 - arrow_width/2, arrow_height, arrow_width));
	texCoords(quad, sf::IntRect(s_width/2 + s_width/8, s_width/8 + s_width/16 + s_width/128, arrow_height, arrow_width));

	quad = &m_arrow_right[0];
	position(quad, sf::IntRect(width - arrow_height - s_width/32, height/2 - arrow_width/2, arrow_height, arrow_width));
	texCoords(quad, sf::IntRect(s_width/2 + s_width/4 + s_width/16, s_width/8 + s_width/16 + s_width/128, arrow_height, arrow_width));

	contents.create(width - getBorderWidth(), height - getBorderWidth());
}


//Window Methods
int WindowSelectable::getRowCount()
{
	return (itemMax + columnMax - 1) / columnMax;
}

int WindowSelectable::getTopRow()
{
	return oy / WINDOW_LINE_HEIGHT;
}

int WindowSelectable::getRowMax()
{
	return (itemMax + columnMax - 1) / columnMax;;
}

void WindowSelectable::setTopRow(int row)
{
	if (row < 0) 
	{
		row = 0;
	}
	if (row > getRowMax() - 1) 
	{
		row = getRowMax() - 1;
	}

	oy = row * WINDOW_LINE_HEIGHT;
}

int WindowSelectable::getPageRowMax()
{
	return (getSize().y - getBorderWidth()) / WINDOW_LINE_HEIGHT;
}

int WindowSelectable::getPageItemMax() 
{
	return getPageRowMax() * columnMax;
}

int WindowSelectable::getBottomRow() 
{
	return getTopRow() + (getPageRowMax() - 1);
}

void WindowSelectable::setBottomRow(int row) 
{
	setTopRow(row - (getPageRowMax() - 1));
}

sf::IntRect WindowSelectable::getItemRect(int id) 
{
	int width = (((getSize().x - getBorderWidth()) + spacing) / columnMax) - spacing;
	return sf::IntRect((id % columnMax) * (width + spacing), (id / columnMax) * WINDOW_LINE_HEIGHT, 
		width, WINDOW_LINE_HEIGHT);
}

void WindowSelectable::cursorDown(bool wrap) 
{
	if ((index < itemMax - columnMax) || (wrap && columnMax == 1)) 
	{
		index = (index + columnMax) % itemMax;
	}
}

void WindowSelectable::cursorUp(bool wrap) 
{
	if ((index >= columnMax) || (wrap && columnMax == 1)) 
	{
		index = (index - columnMax + itemMax) % itemMax;
	}
}

void WindowSelectable::cursorRight(bool wrap) 
{
	if ((columnMax >= 2) && (index < itemMax - 1 || (wrap && getPageRowMax() == 1))) 
	{
		index = (index + 1 + itemMax) % itemMax;
	}
}

void WindowSelectable::cursorLeft(bool wrap) {
	if ((columnMax >= 2) && (index > 0 || (wrap && getPageRowMax() == 1))) 
	{
		index = (index - 1 + itemMax) % itemMax;
	}
}

void WindowSelectable::cursorPageDown() {
	if (getTopRow() + getPageRowMax() < getRowMax()) 
	{
		index = std::min(index + getPageItemMax(), itemMax - 1);
		setTopRow(getTopRow() + getPageRowMax());
	}
}

void WindowSelectable::cursorPageUp() 
{
	if (getTopRow() > 0) 
	{
		index = std::max(index - getPageItemMax(), 0);
		setTopRow(getTopRow() - getPageRowMax());
	}
}

void WindowSelectable::onKeyPressed(KeyPressedEvent e)
{
	if(!m_focused) return;

	if (itemMax > 0) {
		int lastIndex = index;
		if (e.code == sf::Keyboard::Down) {
			cursorDown(sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
		}
		if (e.code == sf::Keyboard::Up) {
			cursorUp(sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
		}
		if (e.code == sf::Keyboard::Right) {
			cursorRight(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
		}
		if (e.code == sf::Keyboard::Left) {
			cursorLeft(sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
		}
		if(e.code == sf::Keyboard::PageUp){
			cursorPageUp();
		}
		if(e.code == sf::Keyboard::PageDown){
			cursorPageDown();
		}
		if (index != lastIndex) {
			cursor.play();
		}
		updateCursor();
	}
}

void WindowSelectable::update(float dt)
{
	time += dt;
}

void WindowSelectable::updateCursor()
{
	int row = 0;
	if (index < 0) 
	{
		setCursorRect(sf::IntRect());
	} 
	else 
	{
		row = index / columnMax;
	}
	if (row < getTopRow()) 
	{
		setTopRow(row);
	}
	if (row > getBottomRow()) 
	{
		setBottomRow(row);
	}

	sf::IntRect rect = getItemRect(index);
	rect.top = (rect.top - oy);
	setCursorRect(rect);
}