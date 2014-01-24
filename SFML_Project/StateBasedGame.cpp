#include "StateBasedGame.h"

#include <iostream>
#include "EmptyTransition.h"
#include "EmptyState.h"
#include "ILoadScreen.h"
#include <sstream>

#ifdef DEBUG
namespace Debug 
{
	bool OVERLAY_ON;
}
#endif //DEBUG

StateBasedGame::StateBasedGame(std::string title)
{
	states = std::hash_map<int, IGameState::ptr>();
	currentState = IGameState::ptr(new EmptyState());
	nextState = nullptr;

	m_load_screen = nullptr;

	enterTransition = nullptr;
	leaveTransition = nullptr;

	winState.mode = sf::VideoMode(1280, 720);
	winState.title = title;
	sf::ContextSettings set;
	//set.antialiasingLevel = 4;
	winState.settings = set;
	winState.vSync = true;
	winState.cursor = false;

#ifdef DEBUG
	winState.cursor = true;
#endif // DEBUG

	winState.fullscreen = false;

	window.create(winState.mode, winState.title, winState.fullscreen ? sf::Style::Fullscreen:sf::Style::Default, winState.settings);
	window.setVerticalSyncEnabled(winState.vSync);
	//window.setFramerateLimit(60);
	window.setMouseCursorVisible(winState.cursor);
}

StateBasedGame::~StateBasedGame()
{
	std::cout << "Destroying SBG" << std::endl;
}

void StateBasedGame::handleEvents(sf::RenderWindow& container)
{
	sf::Event event;
	while (container.pollEvent(event))
	{

		if (event.type == sf::Event::Closed)
			container.close();

		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
			container.close();

		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F11)
		{
			if(!winState.fullscreen) 
			{
				container.create(winState.mode, winState.title, sf::Style::Fullscreen, winState.settings);
				container.setVerticalSyncEnabled(winState.vSync);
				container.setMouseCursorVisible(winState.cursor);
			} 
			else 
			{
				container.create(winState.mode, winState.title, sf::Style::Default, winState.settings);
				container.setVerticalSyncEnabled(winState.vSync);
				container.setMouseCursorVisible(winState.cursor);
			}

			winState.fullscreen = !winState.fullscreen;
		}

#ifdef DEBUG
		if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F12)
		{
			Debug::OVERLAY_ON = !Debug::OVERLAY_ON;
		}
#endif

		if(!enterTransition && !leaveTransition && currentState->isInitialized())
		{
			currentState->handleEvents(event, *this);
		}
	}
}

void StateBasedGame::update(sf::RenderWindow& container, float dt)
{
	if (leaveTransition) {
		leaveTransition->update(dt);
		if (leaveTransition->isComplete()) 
		{
			currentState->leave(*this);
			IGameState::ptr prevState = currentState;
			currentState = nextState;
			if(!currentState->isInitialized())
			{
				std::thread loader([&](){ currentState->init(*this); });
				loader.detach();
			}
			nextState = nullptr;
			leaveTransition = nullptr;
			if (enterTransition) 
			{
				enterTransition->init(currentState, prevState);
			}
		} 
		else 
		{
			return;
		}
	}

	if(!currentState->isInitialized())
	{
		m_load_screen->update(dt);
		return;
	}

	if (enterTransition) {
		enterTransition->update(dt);
		if (enterTransition->isComplete()) {
			MultiResCache::getInstance().Flush();
			currentState->enter(*this);
			enterTransition = nullptr;
		} 
		else 
		{
			return;
		}
	}

	if(!currentState->isPaused())
	{
		currentState->update(container, dt, *this);
	}
}

void StateBasedGame::render(sf::RenderWindow& g, float alpha)
{

	if (leaveTransition) 
	{
		leaveTransition->preRender(g, *this);
	} 
	else if (enterTransition) 
	{
		enterTransition->preRender(g, *this);
	}

	if(!currentState->isInitialized())
	{
		m_load_screen->render(g);
		return;
	}

	if(!currentState->isPaused())
	{
		currentState->render(g, alpha);
	}

	if (leaveTransition) 
	{
		leaveTransition->postRender(g, *this);
	} else if (enterTransition) 
	{
		enterTransition->postRender(g, *this);
	}
}

void StateBasedGame::addState(IGameState::ptr state)
{
	states[state->getID()] = state;
}

IGameState::ptr StateBasedGame::getState(int id)
{
	return states[id];
}

void StateBasedGame::enterState(int id)
{
	enterState(id, std::unique_ptr<Transition>(new EmptyTransition()), std::unique_ptr<Transition>(new EmptyTransition()));
}

void StateBasedGame::enterState(IGameState::ptr state)
{
	enterState(state, std::unique_ptr<Transition>(new EmptyTransition()), std::unique_ptr<Transition>(new EmptyTransition()));
}

void StateBasedGame::enterState(int id, std::unique_ptr<Transition>& enterTrans, std::unique_ptr<Transition>& leaveTrans)
{

	leaveTransition = std::move(leaveTrans);
	enterTransition = std::move(enterTrans);

	nextState = states[id];

	leaveTransition->init(currentState, nextState);
}

void StateBasedGame::enterState(IGameState::ptr state, std::unique_ptr<Transition>& enterTrans, std::unique_ptr<Transition>& leaveTrans)
{
	leaveTransition = std::move(leaveTrans);
	enterTransition = std::move(enterTrans);

	nextState = state;

	leaveTransition->init(currentState, nextState);
}

EventDispatcher& StateBasedGame::getDisptach()
{
	return currentState->getDisptach();
}

ProcessManager& StateBasedGame::getProcessor()
{
	return currentState->getProcessor();
}

void StateBasedGame::setLoadScreen( std::unique_ptr<ILoadScreen>& load_screen )
{
	m_load_screen = std::move(load_screen);
}

sf::RenderWindow& StateBasedGame::getWindow()
{
	return window;
}

WindowState StateBasedGame::getWinState()
{
	return winState;
}

void StateBasedGame::run()
{
	sf::Font font;
	font.loadFromFile("sansation.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setColor(sf::Color::Cyan);

	sf::Clock dtimer;

	const float fps = 120;
	const float dt = 1 / fps;
	float accumulator = 0;

	while (window.isOpen())
	{
		// Store the time elapsed since the last frame began
		const float currentTime = dtimer.restart().asSeconds();
		accumulator += currentTime;

		// Avoid spiral of death and clamp dt, thus clamping
		// how many times the update can be called in
		// a single game loop.
		if(accumulator > 0.2f)
		{
			accumulator = 0.2f;
		}

		while(accumulator > dt)
		{
			update(window, dt);
			accumulator -= dt;
		}

		handleEvents(window);

		const float alpha = accumulator / dt;

		window.clear();
		render(window, alpha);

#ifdef DEBUG
		window.setView(window.getDefaultView());
		text.setString(static_cast<std::ostringstream*>( &(std::ostringstream() << (1/currentTime) ) )->str());
		window.draw(text);
#endif //DEBUG

		window.display();
	}
}

void StateBasedGame::run_threaded()
{
	window.setActive(false);
	
	std::thread render_thread(
		[this]()
		{	
			sf::Clock dtimer;

			sf::Font font;
			font.loadFromFile("sansation.ttf");
			sf::Text text;
			text.setFont(font);
			text.setCharacterSize(24);
			text.setColor(sf::Color::Cyan);

			window.setActive(true);

			window.setVerticalSyncEnabled(winState.vSync);

			while (window.isOpen())
			{
				// Store the time elapsed since the last frame began
				const float currentTime = dtimer.restart().asSeconds();
				window.clear();
				render(window, 0.0f);

#ifdef DEBUG
				window.setView(window.getDefaultView());
				text.setString(static_cast<std::ostringstream*>( &(std::ostringstream() << (1/currentTime) ) )->str());
				window.draw(text);
#endif //DEBUG

				window.display();
			}
		}
	);

	std::thread udpate_thread(
		[this]()
		{
			sf::Clock dtimer;

			const float fps = 120;
			const float dt = 1 / fps;
			float accumulator = 0;

			while (window.isOpen())
			{
				// Store the time elapsed since the last frame began
				const float currentTime = dtimer.restart().asSeconds();
				accumulator += currentTime;

				// Avoid spiral of death and clamp dt, thus clamping
				// how many times the update can be called in
				// a single game loop.
				if(accumulator > 0.2f)
				{
					accumulator = 0.2f;
				}

				while(accumulator > dt)
				{
					window.setTitle(static_cast<std::ostringstream*>( &(std::ostringstream() << (1.f/accumulator) ) )->str());
					update(window, dt);
					accumulator -= dt;
				}

				const float alpha = accumulator / dt;
			}
		}
	);

	while(window.isOpen())
	{
		handleEvents(window);
	}

	udpate_thread.join();
	render_thread.join();
}


