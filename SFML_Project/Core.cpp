#include<boost\python.hpp>

#include "StateBasedGame.h"

#include "PythonState.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(core)
{
	class_<PythonState, std::shared_ptr<PythonState>, boost::noncopyable>("IGameState")
		.def("get_id", pure_virtual(&IGameState::getID))
		.def("init", pure_virtual(&IGameState::init))
		.def("handle_events", pure_virtual(&IGameState::handleEvents))
		.def("update", pure_virtual(&IGameState::update))
		.def("render", pure_virtual(&IGameState::render))
		.def("enter", pure_virtual(&IGameState::enter))
		.def("leave", pure_virtual(&IGameState::leave))
		.def_readwrite("initialized", &IGameState::initialized)
		.def("get_dispatch", &IGameState::getDisptach, return_value_policy<reference_existing_object>())
		;
	class_<EventDispatcher, boost::noncopyable>("EventDispatcher")
		.def("add_listener", &EventDispatcher::addListener)
		.def("trigger_event", &EventDispatcher::triggerEvent<sf::Event::KeyEvent>)
		;

	class_<StateBasedGame, boost::noncopyable>("StateBasedGame", init<std::string>())
		.def("enter_state", static_cast<void(StateBasedGame::*)(int)>(&StateBasedGame::enterState))
		;

	class_<MultiResCache, std::shared_ptr<MultiResCache>, boost::noncopyable>("Resources", no_init)
		.def("instance", &MultiResCache::getInstance, return_value_policy<reference_existing_object>()).staticmethod("instance")
		.def("get_font", &MultiResCache::GetHandle<sf::Font>)
		;

	class_<ResHandle<sf::Font>, Resource<sf::Font>::type, boost::noncopyable>("FontHandle", no_init)
		.def("get", &ResHandle<sf::Font>::get, return_internal_reference<>())
		;
}