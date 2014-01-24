#pragma once

#include "Process.h"

#include <functional>

class CommandBranch : public CloneableProcess<CommandBranch>
{
	std::shared_ptr<Process> branch_false;
	std::function<bool()> predicate;
public:
	CommandBranch(std::function<bool()> condition) { predicate = condition; }

	void update(float dt)
	{
		if(predicate())
		{
			succeed();
		}
		else
		{
			m_pChild = branch_false;
			succeed();
		}
	}

	void attachFalse(std::shared_ptr<Process> false_case)
	{
		branch_false = false_case;
	}

	virtual Process* clone()
	{
		CommandBranch* command_branch = new CommandBranch(*this);

		if(branch_false)
		{
			command_branch->branch_false = std::shared_ptr<Process>(branch_false->clone());
		}

		if(m_pChild)
		{
			command_branch->m_pChild = std::shared_ptr<Process>(m_pChild->clone());
		}

		return command_branch;
	}
};