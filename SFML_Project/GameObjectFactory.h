#pragma once

#include <memory>
#include <string>
#include <iostream>
#include "GameObject.h"
#include "pugixml/pugixml.hpp"

#include "DirectionComponent.h"

class GameObjectFactory
{
private:
	static GameObjectFactory* p_instance;

	GameObjectFactory() {}

	GameObjectFactory(GameObjectFactory&);
	void operator=(GameObjectFactory&);

	~GameObjectFactory() {}

	std::map<std::string, std::function<void(GameObject*, const pugi::xml_node&)>> map;

public:

	static GameObjectFactory& getInstance()
	{
		static GameObjectFactory theInstance;
        p_instance = &theInstance;

		return *p_instance;
	}

	template<class T>
	void Register()
	{
		map[TypeId<T>::name()] = &T::create;
	}

	template<class T>
	void Register(std::string name)
	{
		map[name] = &T::create;
	}

	std::shared_ptr<GameObject> create(const std::string& file)
	{
		pugi::xml_document mapDoc;
		pugi::xml_parse_result result = mapDoc.load_file(file.c_str());

		pugi::xml_node node = mapDoc.child("GameObject");

		auto obj = std::make_shared<GameObject>();

		for(pugi::xml_node& child : node.children())
		{
			std::string name = child.attribute("name").as_string();
			auto func = map[name];

			func(obj.get(), child);
		}

		return obj;
	}
};