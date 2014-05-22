#pragma once

#include "FastDelegate.h"

#include <boost\python.hpp>

using namespace boost::python;

#include <map>
#include <list>

/* A generalized Event Dispatcher,
 * for use in limited scopes (such as within GameObject).
 */
class EventDispatcher
{
	typedef std::list<fastdelegate::DelegateMemento> DelegateList;
	typedef std::map<unsigned int, DelegateList> DelegateMap;

	typedef std::list<object> PyDelegateList;
	typedef std::map<unsigned int, PyDelegateList> PyDelegateMap;
public:
	template<class T>
	bool addListener(fastdelegate::FastDelegate1<T, void> delegate)
	{
		auto& list = listener_map[TypeId<T>::hash_code()];
		if (std::find(list.begin(), list.end(), delegate.GetMemento()) != list.end())
		{
			return false;
		}
		list.push_back(delegate.GetMemento());
		return true;
	}
	
	bool addListener(object clazz, object delegate);
	

	template<class T>
	bool removeListener(fastdelegate::FastDelegate1<T, void> delegate)
	{
		DelegateMap::iterator it = listener_map.find(TypeId<T>::hash_code());
		if (it != listener_map.end())
		{
			auto& list = it->second;
			if (std::find(list.begin(), list.end(), delegate.GetMemento()) != list.end())
			{
				list.remove(delegate.GetMemento());
				return true;
			}
		}
		return false;
	}

	bool removeListener(object clazz, object delegate);

	template<class T>
	void triggerEvent(T event)
	{
		auto it = listener_map.find(TypeId<T>::hash_code());

		if(it != listener_map.end())
		{
			for(fastdelegate::DelegateMemento& delegate : it->second)
			{
				fastdelegate::FastDelegate1<T, void> function;
				function.SetMemento(delegate);
				function(event);
			}
		}

		auto py_it = py_listener_map.find(TypeId<T>::hash_code());

		if (py_it != py_listener_map.end())
		{
			for (object& delegate : py_it->second)
			{
				delegate(event);
			}
		}
	}

protected:
	DelegateMap listener_map;
	PyDelegateMap py_listener_map;
};