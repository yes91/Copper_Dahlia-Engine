#pragma once

#include <list>
#include "Process.h"

class ProcessManager
{
	typedef std::list<std::shared_ptr<Process>> ProcessList;
	ProcessList m_processList;
public:
	// construction
	~ProcessManager();
	// interface
	unsigned int updateProcesses(float dt);
	std::weak_ptr<Process> attachProcess(std::shared_ptr<Process> pProcess);
	void abortAllProcesses(bool immediate);
	// accessors
	unsigned int getProcessCount() const { return m_processList.size(); }
};