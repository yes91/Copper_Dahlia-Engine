#pragma once

#include <functional>
#include <list>
#include <memory>
#include <map>
#include "TypeId.h"

struct FunctionHolderBase
{
	virtual ~FunctionHolderBase() {}

	/*
	virtual int getType() { return -1; }
	*/
};

template<class T>
struct Function : FunctionHolderBase
{
	std::function<void(T)> callback;
	/*
	int getType() { return 0; }
	*/
};

/*
struct PyFunction : FunctionHodlerBase 
{
	object callback;
	int getType() { return 1; }
}
*/

typedef std::map<unsigned int, std::list<std::unique_ptr<FunctionHolderBase>>>::iterator ListenerIter;
typedef std::list<std::unique_ptr<FunctionHolderBase>>::iterator DelegateIter;
