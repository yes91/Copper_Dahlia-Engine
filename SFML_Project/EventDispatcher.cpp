#include "EventDispatcher.h"

bool EventDispatcher::addListener(object clazz, object delegate)
{
	PyDelegateList& list = py_listener_map[extract<unsigned int>(clazz.attr("hash_code")())];

	if (std::find(list.begin(), list.end(), delegate) != list.end())
	{
		return false;
	}
	list.push_back(delegate);
	return true;
}

bool EventDispatcher::removeListener(object clazz, object delegate)
{
	PyDelegateList& list = py_listener_map[extract<unsigned int>(clazz.attr("hash_code")())];

	auto it = std::find(list.begin(), list.end(), delegate);
	if (it != list.end())
	{
		list.erase(it);
		return true;
	}
	return false;
}