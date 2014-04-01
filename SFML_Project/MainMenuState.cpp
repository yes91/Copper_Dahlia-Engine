#include "MainMenuState.h"
#include "KeyPressedEvent.h"
#include "FadeOutTransition.h"
#include "FadeInTransition.h"
#include "WindowCommand.h"
#include "ParallaxPlane.h"

#include <SFML\Graphics.hpp>

#include <iostream>

int MainMenuState::getID()
{
	return -1;
}

MainMenuState::MainMenuState()
{	
	
}

void MainMenuState::init(StateBasedGame& game)
{
	music = std::make_shared<sf::Music>();

	std::vector<std::string> commands;
	commands.push_back("New Game");
	commands.push_back("Continue");
	commands.push_back("Exit");

	win = std::unique_ptr<WindowCommand>(new WindowCommand(commands, 300, 1, 3, dispatch));
	win->setPosition(1280.0f/2 - 150.0f, 720.0f - 270.0f);
	win->setFocus(true);

	background_image = MultiResCache::getInstance().GetHandle<sf::Texture>("res/TitleDesert.png");
	background.setTexture(background_image->get(), true);

	title_tex = MultiResCache::getInstance().GetHandle<sf::Texture>("res/Title.png");
	title.setTexture(title_tex->get());
	title.setPosition(-20.f, 0.f);

	planet_tex = MultiResCache::getInstance().GetHandle<sf::Texture>("res/Planet.png");
	planet.setTexture(planet_tex->get());
	planet.setPosition(550.f - 20.f, 20.f);

	music->openFromFile("res/Audio/BGM/Overland_Blues.ogg");
	music->setLoop(true);

	sandstorm = std::unique_ptr<ParallaxPlane>(new ParallaxPlane("res/Sandstorm.png", sf::Vector2f(1280.0f, 720.0f), sf::Vector2f(8.0f * 60.0f, 8.0f * 60.0f)));
	sandstorm_top = std::unique_ptr<ParallaxPlane>(new ParallaxPlane("res/Sandstorm_top.png", sf::Vector2f(1280.0f, 512.0f), sf::Vector2f(9.0f * 60.0f, 0.f)));

	win->setAction(0, [&](){ game.enterState(0, std::unique_ptr<Transition>(new FadeInTransition()), std::unique_ptr<Transition>(new FadeOutTransition()));});

	glFlush();

	initialized = true;
}

MainMenuState::~MainMenuState()
{
	std::cout << "Destroyed MainMenuState" << std::endl;
}

void MainMenuState::handleEvents(sf::Event event, StateBasedGame& game)
{
	if(event.type == sf::Event::KeyPressed)
	{
		KeyPressedEvent e;
		e.code = event.key.code;
		dispatch.triggerEvent(e);
	}
}

void MainMenuState::update(sf::RenderWindow& container, float dt, StateBasedGame& game)
{
	sandstorm->update(dt);
	sandstorm_top->update(dt);
	win->update(dt);
}

void MainMenuState::render(sf::RenderWindow& g, float alpha)
{	
	g.draw(background);
	g.draw(*sandstorm);
    g.draw(*sandstorm_top);
	g.draw(planet);
	g.draw(title);
	win->draw(g, sf::RenderStates::Default);
}

void MainMenuState::enter(StateBasedGame& game)
{
	music->play();
}

/*void endMusic(std::shared_ptr<sf::Music> music)
{
	sf::Clock clock;
	while(music->getStatus() != sf::Music::Stopped){
		if(clock.getElapsedTime() > sf::seconds(0.1f)){
			music->setVolume(music->getVolume() - 5);
			clock.restart();
		}
		if(music->getVolume() <= 5 ) music->stop();
	}
}*/

void MainMenuState::leave(StateBasedGame& game)
{
	music->stop();
	 //Suicide idiom serves to define states that unload resources after being left, such as a Main Menu.
}