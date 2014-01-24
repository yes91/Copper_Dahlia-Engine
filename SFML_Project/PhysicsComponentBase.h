#pragma once

#include "IPhysicsComponent.h"

template <class T>
class PhysicsComponentBase : public IPhysicsComponent {
public:
	PhysicsComponentBase(GameObject* owner) :IPhysicsComponent(owner) {}

	virtual unsigned int getDerivedType() const { return TypeId<T>::hash_code(); }
};