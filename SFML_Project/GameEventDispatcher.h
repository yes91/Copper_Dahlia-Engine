#pragma once

#include "EventDispatcher.h"

class GameEventDispatcher : public EventDispatcher
{
private:
	static GameEventDispatcher* p_instance;

	GameEventDispatcher() {}

	GameEventDispatcher(GameEventDispatcher&);
	void operator=(GameEventDispatcher&);

	~GameEventDispatcher() {}

public:

	static GameEventDispatcher& getInstance()
	{
		static GameEventDispatcher theInstance;
        p_instance = &theInstance;

		return *p_instance;
	}
};
