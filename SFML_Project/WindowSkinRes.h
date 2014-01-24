#pragma once

#include <SFML/Graphics.hpp>

class WindowSkinRes
{
public:
	bool loadFromFile(std::string name)
	{
		bool load_ok;

		sf::Image image;
		load_ok = image.loadFromFile(name);

		load_ok &= m_main.loadFromImage(image);
		m_main.setSmooth(true);

		width = (int)image.getSize().x;
		height = (int)image.getSize().y;

		load_ok &= m_pattern_tile.loadFromImage(image, sf::IntRect(0, height/2, width/2, height/2));
		m_pattern_tile.setRepeated(true);

		load_ok &= m_top_tile.loadFromImage(image, sf::IntRect(width/2 + width/8, 0, width/4, height/8));
		m_top_tile.setRepeated(true);

		load_ok &= m_right_tile.loadFromImage(image, sf::IntRect(width - width/8, height/8, width/8, height/4));
		m_right_tile.setRepeated(true);

		load_ok &= m_left_tile.loadFromImage(image, sf::IntRect(width/2, height/8, width/8, height/4));
		m_left_tile.setRepeated(true);

		load_ok &= m_bottom_tile.loadFromImage(image, sf::IntRect(width/2 + width/8, height/2 - height/8, width/4, height/8));
		m_bottom_tile.setRepeated(true);

		return load_ok;
	}

	int width;
	int height;

	sf::Texture m_main;
	sf::Texture m_pattern_tile;
	sf::Texture m_top_tile;
	sf::Texture m_left_tile;
	sf::Texture m_right_tile;
	sf::Texture m_bottom_tile;
};