#pragma once

#include "Component.h"

template <class T>
class ComponentBase : public Component {
public:
	ComponentBase(GameObject* owner) :Component(owner) {}

	virtual unsigned int getType() const { return TypeId<T>::hash_code(); }
};