#pragma once

#include "MoveComponentBase.h"
#include "MoveEvent.h"
#include "Random.h"
#include <random>

class RandomMoveComponent : public MoveComponentBase<RandomMoveComponent>
{
public:

	RandomMoveComponent(GameObject* owner) :MoveComponentBase(owner) { move_time = 0.f; rand.SeedMT(std::rand()); }

	void update(sf::RenderTarget& container, float dt);

	sf::Vector2f getMove();

	float getDuration();

	bool equals (const RandomMoveComponent& rhs) const
	{
		return true;
	}

private:
	CRandomMT rand;

	sf::Vector2f current_move;
	float move_time;
};