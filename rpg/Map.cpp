#include "Map.hpp"
#include "../RpgGame.hpp"
#include "TextureManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../../vendor/rapidjson/document.h"
#include "../../vendor/rapidjson/stringbuffer.h"
#include "../../vendor/rapidjson/writer.h"

struct MapLayer {
	int id;
	int height;
	int width;
	std::string name;
	std::vector<int> tiles;
};

struct MapSetting {
	int height;
	int width;
	int tileHeight;
	int tileWidth;
	std::vector<MapLayer> layers;
};

Map::Map() {}

Map::~Map() {}

void Map::LoadMap(std::string path)
{
	// 1. Parse a JSON string into DOM.
	std::ifstream t(path);
	std::string json((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	rapidjson::Document document;
	document.Parse(json.c_str());

	MapSetting setting;
	setting.height = document["height"].GetInt();
	setting.width = document["width"].GetInt();
	setting.tileHeight = document["tileheight"].GetInt();
	setting.tileWidth = document["tilewidth"].GetInt();

	const rapidjson::Value &layers = document["layers"];
	for (rapidjson::SizeType i = 0; i < layers.Size(); i++) { // Uses SizeType instead of size_t
		MapLayer layer;
		layer.id = layers[i]["id"].GetInt();
		layer.height = layers[i]["height"].GetInt();
		layer.width = layers[i]["width"].GetInt();
		layer.name = layers[i]["name"].GetString();

		const rapidjson::Value &tiles = layers[i]["data"];
		for (rapidjson::SizeType j = 0; j < tiles.Size(); j++) { // Uses SizeType instead of size_t
			layer.tiles.push_back(tiles[j].GetInt());
		}
		setting.layers.push_back(layer);
	}

	int tiles_per_row = 8;

	int counter = 0;
	for (int layerNumber = 0; layerNumber < setting.layers.size(); layerNumber++) {
		int tileIndex = 0;
		for (int y = 0; y < setting.height; y++) {
			for (int x = 0; x < setting.width; x++) {

				int tileNumber = setting.layers[layerNumber].tiles[tileIndex];
				tileNumber--; // Map format starts numbering at 1 - we start at 0

				if (tileNumber >= 0) {
					int xpos = ((tileNumber % tiles_per_row) + 0) * setting.tileWidth;
					int ypos = ((tileNumber / tiles_per_row) + 0) * setting.tileHeight;

					std::cout << "x: " << x << " y: " << y << " layer: " << setting.layers[layerNumber].name
					          << " tileNumber: " << tileNumber << " xpos: " << xpos << " ypos: " << ypos << std::endl;

					std::cout << counter << std::endl;
					RpgGame::AddTile(xpos, ypos, x * 64, y * 64);
					counter++;
				}

				tileIndex++;
			}
		}
	}
}