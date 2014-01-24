#pragma once

#include "ComponentBase.h"

class IPhysicsComponent : public ComponentBase<IPhysicsComponent>
{
public:
	IPhysicsComponent(GameObject* owner) :ComponentBase(owner) {}

	virtual sf::FloatRect getGlobalBounds() = 0;

	virtual unsigned int getDerivedType() const = 0;
};