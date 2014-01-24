#pragma once

#include "IGameState.h"
#include "EngineStd.h"
#include <SFML/Audio/Music.hpp>
#include "ResHandle.h"

class WindowCommand;
class ParallaxPlane;

class MainMenuState : public IGameState
{
public:
	MainMenuState();
	~MainMenuState();
	int getID();
	void init(StateBasedGame& game);
	void handleEvents(sf::Event event, StateBasedGame& game);
	void update(sf::RenderWindow& container, float dt, StateBasedGame& game);
	void render(sf::RenderWindow& g, float alpha);
	void enter(StateBasedGame& game);
	void leave(StateBasedGame& game);
private:
	std::shared_ptr<sf::Music> music;
	Resource<sf::Texture>::type background_image;
	Resource<sf::Texture>::type title_tex;
	Resource<sf::Texture>::type planet_tex;
	sf::Sprite background;
	sf::Sprite title;
	sf::Sprite planet;

	std::unique_ptr<ParallaxPlane> sandstorm;
	Resource<sf::Texture>::type sandstorm_tex;
	std::unique_ptr<ParallaxPlane> sandstorm_top;

	std::unique_ptr<WindowCommand> win;
};