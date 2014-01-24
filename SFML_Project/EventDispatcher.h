#pragma once

#include "Function.h"

#include <ctime>

/* A generalized Event Dispatcher,
 * for use in limited scopes (such as within GameObject).
 */
class EventDispatcher
{
public:
	template<class T>
	std::pair<ListenerIter, DelegateIter> addListener(std::function<void(T)> delegate)
	{
		auto& list = listener_map[TypeId<T>::hash_code()];

		Function<T>* func = new Function<T>();
		func->callback = std::move(delegate);
		std::unique_ptr<FunctionHolderBase> holder(func);

		list.push_back(std::move(holder));

		auto it = listener_map.find(TypeId<T>::hash_code());

		return std::pair<ListenerIter, DelegateIter>(it, --(it->second.end()));
	}

	bool removeListener(std::pair<ListenerIter, DelegateIter> delegate)
	{
		if(!listener_map.empty() && delegate.first != listener_map.end() && delegate.second != delegate.first->second.end()) 
		{
			delegate.first->second.erase(delegate.second);
			return true;
		}
		return false;
	}

	template<class T>
	void triggerEvent(T event)
	{
		auto it = listener_map.find(TypeId<T>::hash_code());

		if(it != listener_map.end())
		{
			for(std::unique_ptr<FunctionHolderBase>& func : it->second)
			{
				auto function = static_cast<Function<T>&>(*func).callback;

				function(event);
			}
		}
	}

protected:
	std::map<unsigned int, std::list<std::unique_ptr<FunctionHolderBase>>> listener_map;
};