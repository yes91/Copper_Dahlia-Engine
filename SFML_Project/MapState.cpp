#include "MapState.h"

#include "StateBasedGame.h"
#include "MainMenuState.h"
#include "MapObjectComponent.h"
#include "PlayerControlComponent.h"
#include "ActorPhysicsComponent.h"
#include "ActorSpriteComponent.h"
#include "TriggerEnteredEvent.h"
#include "KeyPressedEvent.h"
#include "DelayProcess.h"
#include "PlayerColorRandomizer.h"
#include "CommandEnd.h"
#include "CommandStart.h"
#include "CommandBranch.h"
#include "MessageProcess.h"
#include "WindowCommand.h"
#include "GameObjectFactory.h"
#include "FadeOutTransition.h"
#include "FadeInTransition.h"
#include "CommandComponent.h"


#include <iostream>
#include <sstream>
#include "RandomMoveComponent.h"

int MapState::getID()
{
	return 0;
}

sf::FloatRect MapState::getCamRect(sf::View camera)
{
	return sf::FloatRect(camera.getCenter().x - camera.getSize().x/2.f, camera.getCenter().y - camera.getSize().y/2.f, camera.getSize().x, camera.getSize().y);
}

MapState::MapState()
	:map("res/data/map")
{
	
}

void MapState::init(StateBasedGame& game)
{
	std::vector<std::string> coms;
	coms.push_back("Change Color");
	coms.push_back("Say Message");

	window = std::make_shared<WindowCommand>(coms, 320, 2, 1, dispatch);

	window->setPosition(100.0f, 100.0f);

	camera = sf::View(sf::FloatRect(0.0f, 0.0f, 1280.0f, 720.0f));

	camera.setCenter(1280.0f/2.f, 720.0f/2.f);

	
	map.Load("testmap2.tmx", dispatch);

	MapChangedEvent e;
	e.bounds = static_cast<sf::Vector2f>(map.GetMapSize());

	sort_pred = [](std::shared_ptr<GameObject> o1, std::shared_ptr<GameObject> o2) { return o1->getPosition().y < o2->getPosition().y; };

	player = game.getObject("Player");

	player->registerEvents(dispatch);

	player->setPosition(100.0f, 100.f);
	
	for(auto& layer : map.GetLayers())
	{
		if(layer.type == tmx::ObjectGroup)
		{
			for(auto& obj : layer.objects)
			{
				if(!obj->has<tmx::MapObjectComponent>()) continue;
				if(obj->get<tmx::MapObjectComponent>().GetType() == "trigger")
				{
					obj->add<CommandComponent>();

					CommandComponent& commands = obj->get<CommandComponent>();

					auto command_start = std::make_shared<CommandStart>(player);
					auto delay = std::make_shared<DelayProcess>(sf::seconds(1.f));
					auto c_bomb = std::make_shared<PlayerColorRandomizer>(player);
					auto branch = std::make_shared<CommandBranch>([](){ return true; });
					auto message = std::make_shared<MessageProcess>(dispatch, sf::seconds(10.f));
					auto command_end = std::make_shared<CommandEnd>(obj, player);

					command_start->attachChild(delay);
					delay->attachChild(c_bomb);
					c_bomb->attachChild(branch);
					branch->attachChild(message); branch->attachFalse(command_end);
					message->attachChild(command_end);

					commands.addCommand(command_start);
				}
			}
		}
	}

	auto mover = std::make_shared<GameObject>();

	mover->add<ActorPhysicsComponent>(sf::Vector2f(0.0f, 0.0f), 400.0f, 0.6f);
	mover->add<DirectionComponent>(DirectionComponent::S);
	mover->add<RandomMoveComponent>();
	mover->add<ActorSpriteComponent>("res/003-Fighter03.png", false);

	mover->registerEvents(dispatch);

	mover->setPosition(100.f, 300.f);

	for(auto& layer : map.GetLayers())
	{
		if(layer.type == tmx::ObjectGroup && layer.dynamic)
		{
			layer.objects.push_back(player);
			layer.objects.push_back(mover);
		}
	}

	window->setAction(0, [&](){ auto c_bomb = std::make_shared<PlayerColorRandomizer>(player); process_man.attachProcess(c_bomb); });

	window->setAction(1, [&](){ auto p = std::make_shared<MessageProcess>(dispatch, sf::seconds(10.f)); process_man.attachProcess(p); });

	dispatch.triggerEvent(e);

	map.UpdateQuadTree(sf::FloatRect(0, 0, map.GetMapSize().x, map.GetMapSize().y));

	//music.openFromFile("res/Audio/BGM/Trial_By_Fire.ogg");
	initialized = true;
}

MapState::~MapState()
{
	std::cout << "Destroyed MapState" << std::endl;
}

void MapState::handleEvents(sf::Event event, StateBasedGame& game)
{
	if(event.type == sf::Event::KeyPressed)
	{
		KeyPressedEvent e;
		e.code = event.key.code;
		dispatch.triggerEvent(e);
	}

	if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::E)
	{
		game.enterState(IGameState::ptr(new MainMenuState()), std::unique_ptr<Transition>(new FadeInTransition()), std::unique_ptr<Transition>(new FadeOutTransition()));
	}
	if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R)
	{
		game.enterState(1);
	}
	if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::M)
	{
		map.Load("testmap3.tmx", dispatch);
		MapChangedEvent e;
		e.bounds = static_cast<sf::Vector2f>(map.GetMapSize());
		dispatch.triggerEvent(e);
		map.UpdateQuadTree(getCamRect(camera));
	}
	if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::D)
	{
		auto p = std::make_shared<DelayProcess>(sf::seconds(2.f));
		auto c_bomb = std::make_shared<PlayerColorRandomizer>(player);
		auto message = std::make_shared<MessageProcess>(dispatch, sf::seconds(10.f));
		c_bomb->attachChild(message);
		p->attachChild(c_bomb);
		process_man.attachProcess(p);
	}
	if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P)
	{
		if(player->has<PlayerControlComponent>())
		{
			PlayerControlComponent& control = player->get<PlayerControlComponent>();
			(control.isPaused()) ? control.resume():control.suspend();

			window->setFocus(!window->isFocused());
		}
	}
}

void MapState::update(sf::RenderWindow& container, float dt, StateBasedGame& game)
{

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSlash))
	{
		camera.zoom(0.98f);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
	{
		camera.zoom(1.02f);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
	{
		container.capture().saveToFile("screencap.png");
	}

	for(auto& layer : map.GetLayers())
	{
		if(layer.type == tmx::ObjectGroup && layer.dynamic)
		{
			std::sort(layer.objects.begin(), layer.objects.end(), sort_pred);
			for(std::shared_ptr<GameObject>& o : layer.objects)
			{
				o->update(container, dt);
			}

			for(std::shared_ptr<GameObject>& o : layer.objects)
			{
				sf::FloatRect bounds = o->get<IPhysicsComponent>().getGlobalBounds();

				auto objs = map.QueryQuadTree(bounds);

				for(std::shared_ptr<GameObject>& other : objs)
				{
					if(other == o) continue;

					sf::FloatRect other_bounds = other->get<IPhysicsComponent>().getGlobalBounds();
					CollisionEvent e;
					TriggerEnteredEvent ev;
					if(other_bounds.intersects(bounds, e.intersection))
					{
						
						ev.process_man = &process_man;
						e.left_top = sf::Vector2f(other_bounds.left, other_bounds.top);
						
						o->triggerEvent(e);
						other->triggerEvent(e);	

						o->triggerEvent(ev);
						other->triggerEvent(ev);
					}
				}
			}
		}
	}

	sf::Vector2f prev = camera.getCenter();
	camera.move(-camera.getCenter().x + int(player->getPosition().x), -camera.getCenter().y + int(player->getPosition().y));

	sf::FloatRect cam_rect = getCamRect(camera);

	if(cam_rect.left + cam_rect.width >=  map.GetMapSize().x || cam_rect.left <= 0.f)
	{
		camera.setCenter(prev.x, camera.getCenter().y);
	}
	if(cam_rect.top + cam_rect.height >=  map.GetMapSize().y || cam_rect.top <= 0.f)
	{
		camera.setCenter(camera.getCenter().x, prev.y);
	}
	
	map.UpdateQuadTree(sf::FloatRect(0, 0, map.GetMapSize().x, map.GetMapSize().y));

	window->update(dt);

	process_man.updateProcesses(dt);
}

void MapState::render(sf::RenderWindow& g, float alpha)
{	
	g.setView(camera);

	map.Draw(g);

	for(auto& layer : map.GetLayers())
	{
		if(layer.type == tmx::ObjectGroup)
		{
			for(std::shared_ptr<GameObject>& o : layer.objects)
			{
				sf::Vector2f prev = o->getPosition();
				o->setPosition(static_cast<float>(static_cast<int>(prev.x)), static_cast<float>(static_cast<int>(prev.y)));
				o->draw(g, sf::RenderStates::Default);
				o->setPosition(prev);
			}
		}
	}

	g.setView(g.getDefaultView());
	//Draw HUD elements here
	window->draw(g, sf::RenderStates::Default);

	DrawEvent e;
	e.target = &g;
	e.states = sf::RenderStates::Default;
	dispatch.triggerEvent(e);

#ifdef DEBUG
	if(!Debug::OVERLAY_ON) return;

	g.setView(camera);
	map.Draw(g, tmx::MapLayer::Debug);
#endif // DEBUG
}

void MapState::enter(StateBasedGame& game)
{
	//music.play();
}

void MapState::leave(StateBasedGame& game)
{
	//music.pause();
}