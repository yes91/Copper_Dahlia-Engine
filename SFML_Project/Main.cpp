
#include <SFML/Graphics.hpp>
#include "StateBasedGame.h"
#include "MainMenuState.h"
#include "MapState.h"
#include "OverlayTestState.h"
#include "GameObjectFactory.h"
#include "DirectionComponent.h"
#include "DiscSpinLoadScreen.h"
#include "ThreadInfo.h"

#ifdef DEBUG
#include "EditorState.h"
#endif // DEBUG

#include <iostream>

#include "TypeId.h"

using namespace std;

namespace thread_info 
{
	std::thread::id main_thread;
}

int main()
{

	thread_info::main_thread = std::this_thread::get_id();

	//std::cout << TypeId<std::tr1::bad_weak_ptr>::qualified_name() << std::endl;
	
	StateBasedGame game("Game");

	game.setLoadScreen(std::unique_ptr<ILoadScreen>(new DiscSpinLoadScreen(game)));

	game.enterState(IGameState::ptr(new MainMenuState()));

	game.addState(IGameState::ptr(new MapState()));

	game.addState(IGameState::ptr(new OverlayTestState()));

	game.run();

	//Threaded fun business
	
	/*std::thread t_main([](){
	StateBasedGame game("Game");

	game.setLoadScreen(std::unique_ptr<ILoadScreen>(new DiscSpinLoadScreen(game)));

	game.enterState(IGameState::ptr(new MainMenuState()));

	game.addState(IGameState::ptr(new MapState()));

	game.addState(IGameState::ptr(new OverlayTestState()));
#ifdef DEBUG
	game.addState(IGameState::ptr(new EditorState(game.getWindow())));
#endif // DEBUG

	game.run();
	});

	std::thread t([](){ 
		StateBasedGame game_t("Game");

		game_t.setLoadScreen(std::unique_ptr<ILoadScreen>(new DiscSpinLoadScreen(game_t)));

		game_t.enterState(IGameState::ptr(new MainMenuState()));

		game_t.addState(IGameState::ptr(new MapState()));

		game_t.addState(IGameState::ptr(new OverlayTestState()));
#ifdef DEBUG
		game_t.addState(IGameState::ptr(new EditorState(game_t.getWindow())));
#endif // DEBUG

		game_t.run(); });

	t.join();
	t_main.join();*/

	return 0;
}