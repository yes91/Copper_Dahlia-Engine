#pragma once

#include "IGameState.h"
class WindowCommand;
#include "GameObject.h"
#include "MapLoader.h"
#include <SFML/Audio/Music.hpp>

class MapState : public IGameState
{
public:
	MapState();
	~MapState();
	int getID();
	void init(StateBasedGame& game);
	void handleEvents(sf::Event event, StateBasedGame& game);
	void update(sf::RenderWindow& container, float dt, StateBasedGame& game);
	void render(sf::RenderWindow& g, float alpha);
	void enter(StateBasedGame& game);
	void leave(StateBasedGame& game);
private:
	sf::FloatRect getCamRect(sf::View camera);

	std::shared_ptr<GameObject> player;
	sf::View camera;
	std::shared_ptr<WindowCommand> window;
	tmx::MapLoader map;
	sf::Music music;

	std::function<bool(std::shared_ptr<GameObject>, std::shared_ptr<GameObject>)> sort_pred;
	std::vector<std::shared_ptr<GameObject>> game_objects;
	std::list<std::shared_ptr<GameObject>> draw_list;
};