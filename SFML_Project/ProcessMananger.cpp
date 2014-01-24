#include "ProcessManager.h"

ProcessManager::~ProcessManager()
{
	m_processList.clear();
}

std::weak_ptr<Process> ProcessManager::attachProcess(std::shared_ptr<Process> pProcess)
{
	m_processList.push_front(pProcess);
	return std::weak_ptr<Process>(pProcess);
}

unsigned int ProcessManager::updateProcesses(float dt)
{
	unsigned short int successCount = 0;
	unsigned short int failCount = 0;
	ProcessList::iterator it = m_processList.begin();
	while (it != m_processList.end())
	{
		// grab the next process
		std::shared_ptr<Process> pCurrProcess = (*it);
		// save the iterator and increment the old one in case we need to remove
		// this process from the list
		ProcessList::iterator thisIt = it;
		++it;
		// process is uninitialized, so initialize it
		if (pCurrProcess->getState() == Process::UNINITIALIZED)
			pCurrProcess->init();
		// give the process an update tick if it’s running
		if (pCurrProcess->getState() == Process::RUNNING)
			pCurrProcess->update(dt);
		// check to see if the process is dead
		if (pCurrProcess->isDead())
		{
			// run the appropriate exit function
			switch (pCurrProcess->getState())
			{
			case Process::SUCCEEDED :
				{
					pCurrProcess->onSuccess();
					std::shared_ptr<Process> pChild = pCurrProcess->removeChild();
					if (pChild)
					{
						attachProcess(pChild);
					}
					else
					{
						pCurrProcess->onChainComplete();
						++successCount; // only counts if the whole chain completed
					}
					break;
				}
			case Process::FAILED :
				{
					pCurrProcess->onFail();
					++failCount;
					break;
				}
			case Process::ABORTED :
				{
					pCurrProcess->onAbort();
					++failCount;
					break;
				}
			}
			// remove the process and destroy it
			m_processList.erase(thisIt);
		}
	}
	return ((successCount << 16) | failCount);
}