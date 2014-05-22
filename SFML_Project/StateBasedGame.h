#pragma once

#include <hash_map>
#include "IGame.h"
#include "MultiResCache.h"
#include <thread>
#include "IGameState.h"

class Transition;
class ILoadScreen;
class EventDispatcher;
class ProcessManager;
class GameObject;

#ifdef DEBUG
namespace Debug 
{
	extern bool OVERLAY_ON;
}
#endif //DEBUG

class StateBasedGame : public IGame
{
public:

	StateBasedGame(std::string title);
	~StateBasedGame();

	void handleEvents(sf::RenderWindow& container);

	void update(sf::RenderWindow& container, float dt);

	void render(sf::RenderWindow& g, float alpha);

	std::shared_ptr<GameObject> getObject(std::string name);

	void addObject(std::string name, std::shared_ptr<GameObject> o);

	void addState(IGameState::ptr state);

	void setLoadScreen(std::unique_ptr<ILoadScreen>& load_screen);

	IGameState::ptr getState(int id);

	EventDispatcher& getDisptach();

	ProcessManager& getProcessor();

	void enterState(int id);

	void enterState(int id, std::unique_ptr<Transition>& enterTrans, std::unique_ptr<Transition>& leaveTrans);

	void enterState(IGameState::ptr state);

	void enterState(IGameState::ptr state, std::unique_ptr<Transition>& enterTrans, std::unique_ptr<Transition>& leaveTrans);

	sf::RenderWindow& getWindow();

	WindowState getWinState();

	void run();

	void run_threaded();

private:
	std::hash_map<int, IGameState::ptr> states;
	IGameState::ptr currentState;
	IGameState::ptr nextState;

	std::unique_ptr<ILoadScreen> m_load_screen; 

	std::unique_ptr<Transition> enterTransition;
	std::unique_ptr<Transition> leaveTransition;

	std::hash_map<std::string, std::shared_ptr<GameObject>> objects;

	sf::RenderWindow window;
	WindowState winState;
};