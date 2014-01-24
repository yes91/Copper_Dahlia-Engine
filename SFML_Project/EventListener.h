#pragma once

#include "Function.h"
#include "EventDispatcher.h"
#include <vector>

class EventListener
{
public:

	EventListener(EventDispatcher& disp)
		:dispatch(disp), delegates()
	{
		
	}

	virtual ~EventListener()
	{
		for(std::pair<ListenerIter, DelegateIter> p : delegates)
		{
			dispatch.removeListener(p);
		}
	}

protected:
	EventDispatcher& dispatch;
	std::vector<std::pair<ListenerIter, DelegateIter>> delegates;
};