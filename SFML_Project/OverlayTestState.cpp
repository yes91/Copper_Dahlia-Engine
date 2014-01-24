#include "OverlayTestState.h"
#include "PlayerControlComponent.h"
#include "ActorPhysicsComponent.h"
#include "ActorSpriteComponent.h"
#include "RandomMoveComponent.h"
#include "StateBasedGame.h"

int OverlayTestState::getID()
{
	return 1;
}

OverlayTestState::OverlayTestState()
{
	
}

void OverlayTestState::init(StateBasedGame& game)
{
	auto player = std::make_shared<GameObject>(dispatch);

	player->add<ActorPhysicsComponent>(sf::Vector2f(0.0f, 0.0f), 400.0f, 0.6f);
	player->add<DirectionComponent>(DirectionComponent::S);
	player->add<PlayerControlComponent>(1);
	player->add<RandomMoveComponent>();
	player->add<ActorSpriteComponent>("res/001-Fighter01.png", false);

	auto mover = std::make_shared<GameObject>(dispatch);

	mover->add<ActorPhysicsComponent>(sf::Vector2f(0.0f, 0.0f), 400.0f, 0.6f);
	mover->add<DirectionComponent>(DirectionComponent::S);
	mover->add<RandomMoveComponent>();
	mover->add<ActorSpriteComponent>("res/003-Fighter03.png", false);


	player->setPosition(1280.0f/2.0f, 720.0f/2.0f);
	mover->setPosition(1280.0f/2.0f, 720.0f/2.0f);

	sort_pred = [](std::shared_ptr<GameObject> o1, std::shared_ptr<GameObject> o2) { return o1->getPosition().y < o2->getPosition().y; };

	game_objects.push_back(player);
	game_objects.push_back(mover);
	draw_list.push_back(player);
	draw_list.push_back(mover);

	for(int i = 0; i < 10; i++)
	{
		auto obj = std::make_shared<GameObject>(dispatch);

		obj->add<ActorPhysicsComponent>(sf::Vector2f(0.0f, 0.0f), 400.0f, 0.6f);
		obj->add<DirectionComponent>(DirectionComponent::S);
		obj->add<RandomMoveComponent>();
		obj->add<ActorSpriteComponent>("res/003-Fighter03.png", false);

		obj->setPosition(sf::Vector2f(1280.0f/2.0f, 720.0f/2.0f));
		game_objects.push_back(obj);
		draw_list.push_back(obj);
	}

	MapChangedEvent e;
	e.bounds = sf::Vector2f(1280, 720);

	dispatch.triggerEvent(e);

	

	initialized = true;
}

OverlayTestState::~OverlayTestState()
{
	
}

void OverlayTestState::handleEvents(sf::Event event, StateBasedGame& game)
{
	if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R)
	{
		game.enterState(0);
	}

	if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::O)
	{
		//mover.remove<IMoveComponent>();
		//mover.add(new PlayerControlComponent(1, 100.0f));
		game_objects[1]->replace(new PlayerControlComponent(game_objects[1].get(), 0));
	}
}

void OverlayTestState::update(sf::RenderWindow& container, float dt, StateBasedGame& game)
{
	draw_list.sort(sort_pred); 
	for(std::shared_ptr<GameObject> o : game_objects)
	{ 
		o->update(container, dt); 
	}
}

//Interpolation - didn't work so well.
/*			sf::Transformable current;
			current.setOrigin(o->getOrigin());
			current.setPosition(o->getPosition());
			current.setRotation(o->getRotation());
			current.setScale(o->getScale());

			sf::Vector2f prev = current.getPosition();
			o->setPosition(o->previous.getPosition() + ((current.getPosition() - o->previous.getPosition()) * alpha));
			g.draw(*o);
			o->setPosition(prev);
			o->previous = current;						*/

void OverlayTestState::render(sf::RenderWindow& g, float alpha)
{	
	for(std::shared_ptr<GameObject> o : draw_list)
	{
		sf::Vector2f prev = o->getPosition();
		o->setPosition(int(prev.x), int(prev.y));
		o->draw(g, sf::RenderStates::Default);
		o->setPosition(prev);
	}
}

void OverlayTestState::enter(StateBasedGame& game)
{

}

void OverlayTestState::leave(StateBasedGame& game)
{

}