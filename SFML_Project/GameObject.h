#pragma once

#include <SFML/Graphics.hpp>
#include "ComponentBase.h"
#include "DrawEvent.h"
#include "EventDispatcher.h"

#include <type_traits>

class Component;

class GameObject : public sf::Transformable
{
public:

	sf::Transformable previous;

	EventDispatcher m_dispatch;
	
	template<class T>
	void addListener(fastdelegate::FastDelegate1<T, void> delegate)
	{
		m_dispatch.addListener<T>(delegate);
	}

	template<class T>
	void addGlobalListener(EventDispatcher& dispatch, fastdelegate::FastDelegate1<T, void> delegate)
	{
		dispatch.addListener<T>(delegate);
	}

	template<class T>
	void removeGlobalListener(EventDispatcher& dispatch, fastdelegate::FastDelegate1<T, void> delegate)
	{
		dispatch.removeListener<T>(delegate);
	}

	void registerEvents(EventDispatcher& dispatch)
	{
		for (Component* comp : components)
		{
			comp->registerEvents(dispatch);
		}
	}

	void unregisterEvents(EventDispatcher& dispatch)
	{
		for (Component* comp : components)
		{
			comp->unregisterEvents(dispatch);
		}
	}

	template<class T>
	bool removeListener(fastdelegate::FastDelegate1<T, void> delegate)
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

	GameObject()
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