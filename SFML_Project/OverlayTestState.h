#pragma once

#include "IGameState.h"
#include "GameObject.h"

class OverlayTestState : public IGameState
{
public:
	OverlayTestState();
	~OverlayTestState();
	int getID();
	void init(StateBasedGame& game);
	void handleEvents(sf::Event event, StateBasedGame& game);
	void update(sf::RenderWindow& container, float dt, StateBasedGame& game);
	void render(sf::RenderWindow& g,float alpha);
	void enter(StateBasedGame& game);
	void leave(StateBasedGame& game);
private:
	std::function<bool(std::shared_ptr<GameObject>, std::shared_ptr<GameObject>)> sort_pred;
	std::vector<std::shared_ptr<GameObject>> game_objects;
	std::list<std::shared_ptr<GameObject>> draw_list;
};