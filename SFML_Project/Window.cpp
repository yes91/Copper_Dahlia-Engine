#include "Window.h"
#include "Util.h"

using namespace util;

Window::Window(int width, int height, EventDispatcher& disp)
	:EventListener(disp)
{

	m_skin = MultiResCache::getInstance().GetHandle<WindowSkinRes>("res/system/Window_c_00.png");

	m_border.setPrimitiveType(sf::Quads);
	m_border.resize(4 * 4);

	m_background.setPrimitiveType(sf::Quads);
	m_background.resize(1 * 4);

	m_pattern.setPrimitiveType(sf::Quads);
	m_pattern.resize(1 * 4);

	m_border_top.setPrimitiveType(sf::Quads);
	m_border_top.resize(1 * 4);

	m_border_right.setPrimitiveType(sf::Quads);
	m_border_right.resize(1 * 4);

	m_border_left.setPrimitiveType(sf::Quads);
	m_border_left.resize(1 * 4);

	m_border_bottom.setPrimitiveType(sf::Quads);
	m_border_bottom.resize(1 * 4);

	m_border_width = m_skin->get().width/4;

	oy = 0;

	resize(width, height);
	
	setOpacity(0.80f);
}

void Window::setOpacity(float percent)
{
	int alpha = (int)(percent*255.0f);

	sf::Vertex* quad = &m_pattern[0];

	color(quad, sf::Color(quad[0].color.r, quad[0].color.g, quad[0].color.b, alpha));

	quad = &m_background[0];

	color(quad, sf::Color(quad[0].color.r, quad[0].color.g, quad[0].color.b, alpha));
}

void Window::resize(int width, int height)
{
	size = sf::Vector2u(width, height);

	int s_width = m_skin->get().width;
	int s_height = m_skin->get().height;

	//Top left corner
	sf::Vertex* quad = &m_border[0];
	position(quad, sf::IntRect(0, 0, s_width/8, s_height/8));
	texCoords(quad, sf::IntRect(s_width/2, 0, s_width/8, s_height/8));

	//Top right corner
	quad = &m_border[1*4];
	position(quad, sf::IntRect(width - s_width/8, 0, s_width/8, s_height/8));
	texCoords(quad, sf::IntRect(s_width - s_width/8, 0, s_width/8, s_height/8));


	//Bottom right corner
	quad = &m_border[2*4];
	position(quad, sf::IntRect(width - s_width/8, height - s_height/8, s_width/8, s_height/8));
	texCoords(quad, sf::IntRect(s_width - s_width/8, s_height/2 - s_height/8, s_width/8, s_height/8));

	//Bottom left corner
	quad = &m_border[3*4];
	position(quad, sf::IntRect(0, height - s_height/8, s_width/8, s_height/8));
	texCoords(quad, sf::IntRect(s_width/2, s_height/2 - s_height/8, s_width/8, s_height/8));

	//Top middle bar
	quad = &m_border_top[0];
	position(quad, sf::IntRect(s_width/8, 0, width - s_width/4, s_height/8));
	texCoords(quad, sf::IntRect(0, 0, width, s_height/8));

	//left side bar
	quad = &m_border_left[0];
	position(quad, sf::IntRect(0, s_height/8, s_width/8, height - s_height/4));
	texCoords(quad, sf::IntRect(0, 0, s_width/8, height));

	//right side bar
	quad = &m_border_right[0];
	position(quad, sf::IntRect(width - s_width/8, s_height/8, s_width/8, height - s_height/4));
	texCoords(quad, sf::IntRect(0, 0, s_width/8, height));

	//Bottom bar
	quad = &m_border_bottom[0];
	position(quad, sf::IntRect(s_width/8, height - s_height/8, width - s_width/4, s_height/8));
	texCoords(quad, sf::IntRect(0, 0, width, s_height/8));

	//background
	quad = &m_background[0];
	position(quad, sf::IntRect(s_width/64, s_height/64, width - s_width/32, height - s_height/32));
	texCoords(quad, sf::IntRect(0, 0, s_width/2, s_height/2));

	//Pattern
	quad = &m_pattern[0];
	position(quad, sf::IntRect(s_width/32, s_height/32, width - s_width/32, height - s_height/32));
	texCoords(quad, sf::IntRect(s_width/32, s_height/32, width - s_width/32, height - s_height/32));
}

void Window::draw(sf::RenderTarget& target, sf::RenderStates states)
{
	// apply the entity's transform -- combine it with the one that was passed by the caller
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

	states.texture = &m_skin->get().m_main;
	target.draw(m_background, states);

	states.texture = &m_skin->get().m_pattern_tile;
	target.draw(m_pattern, states);

	states.texture = &m_skin->get().m_main;
	target.draw(m_border, states);

	states.texture = &m_skin->get().m_top_tile;
	target.draw(m_border_top, states);

	states.texture = &m_skin->get().m_right_tile;
	target.draw(m_border_right, states);

	states.texture = &m_skin->get().m_left_tile;
	target.draw(m_border_left, states);

	states.texture = &m_skin->get().m_bottom_tile;
	target.draw(m_border_bottom, states);
}