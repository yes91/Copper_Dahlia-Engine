#pragma once

#include <SFML/Graphics.hpp>
#include "ComponentBase.h"
#include "DrawEvent.h"
#include "EventDispatcher.h"

#include <type_traits>

class Component;

class GameObject : public sf::Transformable, public EventListener
{
public:

	sf::Transformable previous;

	EventDispatcher m_dispatch;
	
	template<class T>
	std::pair<ListenerIter, DelegateIter> addListener(std::function<void(T)> delegate)
	{
		return m_dispatch.addListener<T>(delegate);
	}

	template<class T>
	void addGlobalListener(std::function<void(T)> delegate)
	{
		delegates.push_back(dispatch.addListener<T>(delegate));
	}

	bool removeListener(std::pair<ListenerIter, DelegateIter> delegate)
	{
		return m_dispatch.removeListener(delegate);
	}

	template<class T>
	void triggerEvent(T event)
	{
		m_dispatch.triggerEvent(event);
	}

	std::vector<Component*> components;

	template <class T>
	const T& getConst() 
	{
		Iterator it = std::find_if(components.begin(), components.end(), predicate<T>());
		if(it != components.end())
		{
			return *static_cast<T*>(*it);
		}
	}

	template <class T>
	T& get() 
	{
		Iterator it = std::find_if(components.begin(), components.end(), predicate<T>());
		if(it != components.end())
		{
			return *static_cast<T*>(*it);
		}
	}

	template<class T>
	void remove()
	{
		Iterator it = std::find_if(components.begin(), components.end(), predicate<T>());
		if(it != components.end())
		{
			components.erase(it);
		}
	}

	template<typename T>
	void replace(T* comp)
	{
		Iterator it = std::find_if(components.begin(), components.end(), [comp](Component* c){ return c->getType() == comp->getType(); });
		if(it != components.end())
		{
			if(comp->getDerivedType() == (*it)->getDerivedType())
			{
				if(comp->equals(*static_cast<T*>(*it)))
				{
					delete comp;
					return;
				}
				else
				{
					components.erase(it);
					components.push_back(comp);
				}
			}
			else
			{
				components.erase(it);
				components.push_back(comp);
			}
		}
		else
		{
			delete comp;
		}
	}

	template<class T, typename... Arguments, class = std::enable_if<(sizeof...(Arguments) > 0)>::type>
	bool add(Arguments... params)
	{
		Component* comp;

		comp = new T(this, params...);

		Iterator it = std::find_if(components.begin(), components.end(), [&](Component* c){ return c->getType() == comp->getType(); });
		if(it != components.end())
		{
			return false;
		}
		components.push_back(comp);
		return true;
	}

	template<class T, typename... Arguments>
	bool add()
	{
		Component* comp;

		comp = new T(this); 

		Iterator it = std::find_if(components.begin(), components.end(), [&](Component* c){ return c->getType() == comp->getType(); });
		if(it != components.end())
		{
			return false;
		}
		components.push_back(comp);
		return true;
	}

	template<class T>
	bool has()
	{
		Iterator it = std::find_if(components.begin(), components.end(), predicate<T>());
		return it != components.end();
	}

	virtual void update(sf::RenderTarget& container, float dt) 
	{  
		for(Component* c : components)
		{
			c->update(container, dt);
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)
	{
		states.transform *= getTransform();
		DrawEvent e;
		e.target = &target;
		e.states = states;
		this->triggerEvent(e);
	}

	GameObject(EventDispatcher& disp)
		:EventListener(disp)
	{
		
	}

	virtual ~GameObject() {}

private:

	template<class T>
	std::function<bool(Component*)> predicate() 
	{
		return [](Component* c) -> bool 
		{ 
			if(TypeId<T>::hash_code() == c->getType())
			{
				return true;
			}

			return c->getDerivedType() == TypeId<T>::hash_code();
		};
	} 

	typedef std::vector<Component*>::iterator Iterator;
};