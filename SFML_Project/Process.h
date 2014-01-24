#pragma once

#include <memory>

class Process
{
	friend class ProcessManager;
public:
	enum State
	{
		// Processes that are neither dead nor alive
		UNINITIALIZED = 0, // created but not running
		REMOVED, // removed from the process list but not destroyed; this can
		// happen when a process that is already running is parented
		// to another process.
		// Living processes
		RUNNING, // initialized and running
		PAUSED, // initialized but paused
		// Dead processes
		SUCCEEDED, // completed successfully
		FAILED, // failed to complete
		ABORTED, // aborted; may not have started
	};

private:
	State m_state; // the current state of the process
protected:
	std::shared_ptr<Process> m_pChild; // the child process, if any

public:
	Process() { m_state = UNINITIALIZED; }
	virtual ~Process() {}

	virtual void init() { m_state = RUNNING; }
	virtual void update(float dt) = 0;
	virtual void onSuccess() {}
	virtual void onChainComplete() {}
	virtual void onFail() {}
	virtual void onAbort() {}

	virtual Process* clone() = 0;

public:
	// Functions for ending the process.
	void succeed() { m_state = SUCCEEDED; }
	void fail() { m_state = FAILED; };
	// pause
	void pause();
	void unPause();
	// accessors
	State getState() const { return m_state; }
	bool isAlive() const {return (m_state == RUNNING || m_state == PAUSED);}
	bool isDead() const
	{
		return (m_state == SUCCEEDED || m_state == FAILED || m_state == ABORTED);
	}
	bool isRemoved() const { return (m_state == REMOVED); }
	bool isPaused() const { return m_state == PAUSED; }
	// child functions
	void attachChild(std::shared_ptr<Process> pChild) { m_pChild = pChild; }
	std::shared_ptr<Process> removeChild() 
	{ 
		std::shared_ptr<Process> p; 
		m_pChild.swap(p); 
		return p; 
	} // releases ownership of the child
	std::shared_ptr<Process> peekChild() { return m_pChild; } // doesn't release
	// ownership of child
private:
	void setState(State newState) { m_state = newState; }
};

template<class Derived>
class CloneableProcess : public Process
{
public:
	CloneableProcess() :Process() {}

	virtual Process* clone() 
	{
		Derived* d = new Derived(static_cast<Derived const &>(*this));
		if(m_pChild)
		{
			d->m_pChild = std::shared_ptr<Process>(m_pChild->clone());
		}

		return d;
	}
};