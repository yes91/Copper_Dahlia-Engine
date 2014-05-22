#pragma once

#include <boost\python.hpp>

class AcquireGIL
{
public:
	inline AcquireGIL(){
		state = PyGILState_Ensure();
	}

	inline ~AcquireGIL(){
		PyGILState_Release(state);
	}
private:
	PyGILState_STATE state;
};