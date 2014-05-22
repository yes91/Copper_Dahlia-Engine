#pragma once

#include "Function.h"
#include <vector>

class EventDispatcher;

class EventListener
{
public:

	EventListener(EventDispatcher& disp)
		:dispatch(disp)
	{
		
	}

	virtual ~EventListener()
	{
	}

protected:
	EventDispatcher& dispatch;
};