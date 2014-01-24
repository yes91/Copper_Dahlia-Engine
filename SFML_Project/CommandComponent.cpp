#include "CommandComponent.h"

#include "GameObject.h"
#include "TriggerEnteredEvent.h"
#include "CommandEndedEvent.h"
#include "ProcessManager.h"

#include <iostream>

CommandComponent::CommandComponent(GameObject* owner)
	:ComponentBase(owner)
{
	delegates.push_back(owner->addListener<TriggerEnteredEvent>([this](TriggerEnteredEvent e){ this->onTriggerEntered(e);}));
	delegates.push_back(owner->addListener<CommandEndedEvent>([this](CommandEndedEvent e){ this->onCommandEnd(e); }));
	running = false;
}

void CommandComponent::addCommand(std::shared_ptr<Process> p)
{
	if(!root_process)
	{
		root_process = p;
	}
	else
	{
		root_process->attachChild(p);
	}
}

void CommandComponent::update(sf::RenderTarget& container, float dt)
{

}

void CommandComponent::onCommandEnd(CommandEndedEvent e)
{
	running = false;
}

void CommandComponent::onTriggerEntered(TriggerEnteredEvent e)
{
	//if(conditions.met())
	if(!running) 
	{
		auto process = std::shared_ptr<Process>(root_process->clone());
		e.process_man->attachProcess(process);
		running = true;
	}
}