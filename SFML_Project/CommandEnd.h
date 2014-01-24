#pragma once

#include "Process.h"
#include "CommandEndedEvent.h"
#include "GameObject.h"

#include "PlayerControlComponent.h"

class CommandEnd : public CloneableProcess<CommandEnd>
{
	std::weak_ptr<GameObject> self;
	std::shared_ptr<GameObject> player;
public:
	CommandEnd(std::shared_ptr<GameObject> command_obj, std::shared_ptr<GameObject> player) 
	{ 
		self = std::weak_ptr<GameObject>(command_obj); 
		this->player = player;
	}

protected:
	void update(float dt)
	{
		succeed();
	}

	void onChainComplete()
	{
		if(self.expired()) return;

		CommandEndedEvent e;
		self.lock()->triggerEvent(e);

		if(player->has<PlayerControlComponent>()) 
			player->get<PlayerControlComponent>().resume();
	}
};