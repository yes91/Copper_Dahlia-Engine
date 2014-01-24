#pragma once

#include "ComponentBase.h"

class Process;
struct TriggerEnteredEvent;
struct CommandEndedEvent;

class CommandComponent : public ComponentBase<CommandComponent>
{
public:
	CommandComponent(GameObject* owner);
	
	void addCommand(std::shared_ptr<Process> p);

	void update(sf::RenderTarget& container, float dt);
private:
	void onTriggerEntered(TriggerEnteredEvent e);
	void onCommandEnd(CommandEndedEvent e);

private:
	bool running;
	std::shared_ptr<Process> root_process;
};