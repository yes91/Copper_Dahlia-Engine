
#include <SFML/Graphics.hpp>
#include "StateBasedGame.h"
#include "MainMenuState.h"
#include "MapState.h"
#include "PythonState.h"
#include "OverlayTestState.h"
#include "GameObjectFactory.h"
#include "DirectionComponent.h"
#include "DiscSpinLoadScreen.h"
#include "ThreadInfo.h"

#include <iostream>

#include <fstream>

#include "TypeId.h"

#include <boost/python.hpp>

#define ADD_MODULE(name) if (PyImport_AppendInittab(#name, PyInit_##name) == -1) { \
	throw std::runtime_error("Failed to add #name to the interpreter's " \
	"builtin modules"); } \

#define DECLARE_MODULE(name) extern "C" PyObject* PyInit_##name(void)

using namespace std;

using namespace boost::python;

namespace thread_info
{
	std::thread::id main_thread;
}

DECLARE_MODULE(core);

DECLARE_MODULE(sfml);


int main()
{

	thread_info::main_thread = std::this_thread::get_id();
	
	ADD_MODULE(core);

	ADD_MODULE(sfml);

	cout << TypeId<sf::RectangleShape>::hash_code() << endl;

	Py_Initialize();

	PyEval_InitThreads();

	object main;

	object global;

	try {
		// Retrieve the main module.
		main = import("__main__");

		// Retrieve the main module's namespace
		global = main.attr("__dict__");
		
		// Define greet function in Python.
		std::ifstream t;
		t.open("test_state.py");
		std::stringstream buffer;
		buffer << t.rdbuf();
		object result = exec(buffer.str().c_str(), global, global);

	}
	catch ( error_already_set )
	{
		PyErr_Print();
	}
	catch (std::exception e)
	{
		cout << e.what() << endl;
	}
	

	//std::cout << TypeId<std::tr1::bad_weak_ptr>::qualified_name() << std::endl;
	
	StateBasedGame game("Game");

	game.setLoadScreen(std::unique_ptr<ILoadScreen>(new DiscSpinLoadScreen(game)));

	game.enterState(IGameState::ptr(new MainMenuState()));
    
	game.addState(IGameState::ptr(new MapState()));

	game.addState(IGameState::ptr(new OverlayTestState()));

	object instance;
	try {
		object test_state = global["TestState"];
		instance = test_state();
		std::shared_ptr<PythonState> p = extract<std::shared_ptr<PythonState>>(instance);

		game.enterState(p);
	}
	catch (error_already_set)
	{
		PyErr_Print();
	}
	
	

	game.addObject("Player", GameObjectFactory::getInstance().create("res/data/Player.xml"));

	try {
		game.run();
	}
	catch (error_already_set)
	{
		PyErr_Print();
		getchar();
	}
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