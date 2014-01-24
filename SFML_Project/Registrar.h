#pragma once

#include "GameObjectFactory.h"
#include <string>

template<class T>
class Registrar
{
public:
	Registrar()
	{
		GameObjectFactory::getInstance().Register<T>();
	}

	Registrar(std::string name)
	{
		GameObjectFactory::getInstance().Register<T>(name);
	}
};