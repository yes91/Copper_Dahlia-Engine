#pragma once

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

namespace tmx
{
	class MapObject;
	struct MapTile;
}

namespace tmx 
{

	typedef std::vector<std::shared_ptr<GameObject>> MapObjects;
	typedef std::vector<MapTile> MapTiles;

	enum MapLayerType
	{
		Layer,
		ObjectGroup,
		ImageLayer
	};

	//represents a layer of tiles, corresponding to a tmx layer, object group or image layer
	struct MapLayer
	{
		MapLayer(MapLayerType layerType) : opacity(1.f), visible(true), dynamic(false), type(layerType){};
		std::string name;
		float opacity; //range 0 - 1
		bool visible, dynamic; //dynamic layers contain sprites which need their order sorting, such as players / NPCs
		MapTiles tiles;
		MapObjects objects; //vector of objects if layer is object group
		MapLayerType type;
		std::map <std::string, std::string> properties;
		std::vector<sf::VertexArray> vertexArrays;
		sf::RenderStates States;

		//used for drawing specific layers
		enum DrawType
		{
			Front,
			Back,
			Debug,
			All
		};
	};
};