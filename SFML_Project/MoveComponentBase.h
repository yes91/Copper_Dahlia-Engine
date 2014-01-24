#pragma once

#include "IMoveComponent.h"

template <class T>
class MoveComponentBase : public IMoveComponent {
public:
	MoveComponentBase(GameObject* owner) :IMoveComponent(owner) {}

	virtual unsigned int getDerivedType() const { return TypeId<T>::hash_code(); }
};