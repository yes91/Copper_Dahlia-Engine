#pragma once

#include <functional>
#include <list>
#include <memory>
#include <map>
#include "TypeId.h"

struct FunctionHolderBase
{
	virtual ~FunctionHolderBase() {}
};

template<class T>
struct Function : FunctionHolderBase
{
	std::function<void(T)> callback;
};

typedef std::map<unsigned int, std::list<std::unique_ptr<FunctionHolderBase>>>::iterator ListenerIter;
typedef std::list<std::unique_ptr<FunctionHolderBase>>::iterator DelegateIter;
