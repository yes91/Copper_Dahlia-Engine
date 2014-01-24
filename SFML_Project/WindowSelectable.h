#pragma once
#include "Window.h"
#include <memory>
#include <SFML/Audio/Sound.hpp>

struct KeyPressedEvent;

class WindowSelectable : public Window
{

public:
	WindowSelectable(int width, int height, EventDispatcher& disp);

	void resize(int width, int height);

	int ItemMax() const { return itemMax; }
	void ItemMax(int val) { itemMax = val; }

	int ColumnMax() const { return columnMax; }
	void ColumnMax(int val) { columnMax = val; }

	int Index() const { return index; }

	void setCursorPos(int id) { index = id; }

	bool isFocused() { return m_focused; }

	void setFocus(bool focus) {focus ? index = 0:index = -1; updateCursor(); m_focused = focus; }

	void update(float dt);

	void draw(sf::RenderTarget& target, sf::RenderStates states);

protected: 
	void setCursorRect(sf::IntRect cursorRect);

	int getRowCount();

	int getTopRow();

	int getRowMax();

	void setTopRow(int row);

	int getPageRowMax();

	int getPageItemMax();

	int getBottomRow();

	void setBottomRow(int row);

	sf::IntRect getItemRect(int id);

	void cursorDown(bool wrap);

	void cursorUp(bool wrap);

	void cursorRight(bool wrap);

	void cursorLeft(bool wrap);

	void cursorPageDown();

	void cursorPageUp();

	void updateCursor();

	void onKeyPressed(KeyPressedEvent e);

	float time;

	
	sf::Sound cursor;
	sf::Sound cursor_select;
	Resource<sf::SoundBuffer>::type cursor_sfx;
	Resource<sf::SoundBuffer>::type cursor_select_sfx;

	//Cursor
	sf::VertexArray m_cursor;

private:
	int itemMax;
	int columnMax;
	int index;
	int spacing;

	bool m_focused;

	sf::VertexArray m_arrow_up;
	sf::VertexArray m_arrow_down;
	sf::VertexArray m_arrow_left;
	sf::VertexArray m_arrow_right;
};