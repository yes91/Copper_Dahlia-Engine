#pragma once

#include "IGameState.h"
#include <TGUI/TGUI.hpp>
#include "MapLoader.h"

#include "GameObject.h"

class EditorState : public IGameState
{
public:
	EditorState(sf::RenderWindow& window);
	~EditorState();
	int getID();
	void init(StateBasedGame& game);
	void handleEvents(sf::Event event, StateBasedGame& game);
	void update(sf::RenderWindow& container, float dt, StateBasedGame& game);
	void render(sf::RenderWindow& g, float alpha);
	void enter(StateBasedGame& game);
	void leave(StateBasedGame& game);

private:

	sf::FloatRect getCamRect(sf::View camera);

	tgui::Gui gui;

	tmx::MapLoader map;

	std::vector<std::shared_ptr<GameObject>> game_objects;

	sf::View camera;
	sf::Vector2i previous;
};
