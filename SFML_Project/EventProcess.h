#pragma once

#include "Process.h"
#include "StateBasedGame.h"
#include "EventListener.h"

template<class Derived>
class EventProcess : public CloneableProcess<Derived>, public EventListener
{
public:
	EventProcess(EventDispatcher& disp)
		:CloneableProcess(), EventListener(disp)
	{

	}
};