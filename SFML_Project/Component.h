#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include "EventListener.h"
#include <vector>

class GameObject;

class Component {
public:
	Component(GameObject* owner) { this->m_pOwner = owner; }

	virtual ~Component();

	virtual void update(sf::RenderTarget& container, float dt) = 0;
	virtual unsigned int getType() const = 0;
	virtual unsigned int getDerivedType() const { return 0; }
protected:
	GameObject* m_pOwner;
	std::vector<std::pair<ListenerIter, DelegateIter>> delegates;
};