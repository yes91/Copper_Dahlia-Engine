#pragma once

#include "Process.h"
#include "GameObject.h"

#include "PlayerControlComponent.h"

class CommandStart : public CloneableProcess<CommandStart>
{
	std::shared_ptr<GameObject> player;
public:
	CommandStart(std::shared_ptr<GameObject> player)
	{
		this->player = player;
	}

	void update(float dt)
	{
		succeed();
	}

	void onSuccess()
	{
		if(player->has<PlayerControlComponent>())
			player->get<PlayerControlComponent>().suspend();
	}
};