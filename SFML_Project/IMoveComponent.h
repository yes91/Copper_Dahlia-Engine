#pragma once

#include "ComponentBase.h"
#include <SFML/System/Vector2.hpp>

class IMoveComponent : public ComponentBase<IMoveComponent>
{
public:
	IMoveComponent(GameObject* owner) :ComponentBase(owner) {}
	virtual sf::Vector2f getMove() = 0;
	virtual bool isRun() { return false; }
	virtual float getDuration() { return 1.f/60.f; }
	virtual unsigned int getDerivedType() const = 0;
};