#include<boost\python.hpp>

#include <SFML/Graphics.hpp>

#include "TypeId.h"

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(draw_vertex_array, sf::RenderWindow::draw, 3, 4)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(draw_drawable, sf::RenderWindow::draw, 1, 2)

BOOST_PYTHON_MODULE(sfml)
{

	class_<sf::RenderWindow, boost::noncopyable>("RenderWindow")
		.def("draw", static_cast<void(sf::RenderWindow::*)(const sf::Drawable&, const sf::RenderStates&)>(&sf::RenderWindow::draw), draw_drawable())
		;
	class_<sf::Color>("Color", init<sf::Uint8, sf::Uint8, sf::Uint8>())
		.def_readonly("cyan", &sf::Color::Cyan)
		;
	class_<sf::Vector2f>("Vector2f", init<float, float>())
		;
	class_<sf::Drawable, boost::noncopyable>("Drawable", no_init)
		;
	class_<sf::RectangleShape, bases<sf::Drawable>>("RectangleShape", init<const sf::Vector2f&>())
		.def("set_fill", &sf::RectangleShape::setFillColor)
		.def("set_pos", static_cast<void(sf::RectangleShape::*)(float x, float y)>(&sf::RectangleShape::setPosition))
		.def("set_size", &sf::RectangleShape::setSize)
		;
	//.def("hash_code", &TypeId<sf::RectangleShape>::hash_code).staticmethod("hash_code") -- It works!!
	class_<sf::String>("sf_String", init<std::string>())
		;

	implicitly_convertible<sf::String, std::string>();
	implicitly_convertible<std::string, sf::String>();

	class_<sf::Text, bases<sf::Drawable>>("Text")
		.def("set_color", &sf::Text::setColor)
		.def("set_string", &sf::Text::setString)
		.def("set_pos", static_cast<void(sf::Text::*)(float x, float y)>(&sf::Text::setPosition))
		.def("set_font", &sf::Text::setFont)
		;

	class_<sf::Font>("Font")
		;

	enum_<sf::Event::EventType>("EventType")
		.value("KeyReleased", sf::Event::KeyReleased)
		.value("MouseButtonPressed", sf::Event::MouseButtonPressed)
		;

	class_<sf::Event>("Event")
		.def_readonly("type", &sf::Event::type)
		.def_readonly("key", &sf::Event::key)
		.def_readonly("mouse_button", &sf::Event::mouseButton)
		;

	class_<sf::Event::KeyEvent>("KeyEvent")
		.def_readonly("code", &sf::Event::KeyEvent::code)
		.def("hash_code", &TypeId<sf::Event::KeyEvent>::hash_code).staticmethod("hash_code")
		;
	class_<sf::Event::MouseButtonEvent>("MouseButtonEvent")
		.def_readonly("button", &sf::Event::MouseButtonEvent::button)
		.def_readonly("x", &sf::Event::MouseButtonEvent::x)
		.def_readonly("y", &sf::Event::MouseButtonEvent::y)
		;

	enum_<sf::Mouse::Button>("MouseButton")
		.value("left", sf::Mouse::Left)
		;

	class_<sf::Keyboard, boost::noncopyable>("Keyboard", no_init)
		.def("key_pressed", &sf::Keyboard::isKeyPressed).staticmethod("key_pressed")
		;

	enum_<sf::Keyboard::Key>("Key")
		.value("W", sf::Keyboard::W)
		.value("A", sf::Keyboard::A)
		.value("S", sf::Keyboard::S)
		.value("D", sf::Keyboard::D)
		.value("R", sf::Keyboard::R)
		;
}