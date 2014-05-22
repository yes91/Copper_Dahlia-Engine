#include "CommandComponent.h"

#include "GameObject.h"
#include "TriggerEnteredEvent.h"
#include "CommandEndedEvent.h"
#include "ProcessManager.h"

#include <iostream>

CommandComponent::CommandComponent(GameObject* owner)
	:ComponentBase(owner)
{
	owner->addListener<TriggerEnteredEvent>(fastdelegate::MakeDelegate(this, &CommandComponent::onTriggerEntered));
	owner->addListener<CommandEndedEvent>(fastdelegate::MakeDelegate(this, &CommandComponent::onCommandEnd));
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