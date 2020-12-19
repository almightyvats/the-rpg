#include "Map.hpp"
#include "../vendor/rapidjson/document.h"
#include "../vendor/rapidjson/stringbuffer.h"
#include "../vendor/rapidjson/writer.h"
#include "RpgGame.hpp"
#include "TextureManager.hpp"
#include "ecs/Components.hpp"
#include "ecs/ecs.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

extern Manager manager;

Map::Map(std::string path, int mapScale)
{
	std::ifstream t(path);

	std::string json((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	rapidjson::Document document;
	document.Parse(json.c_str());

	setting.mapScale = mapScale;
	setting.height = document["height"].GetInt();
	setting.width = document["width"].GetInt();
	setting.tileHeight = document["tileheight"].GetInt();
	setting.tileWidth = document["tilewidth"].GetInt();
	setting.tileSize = setting.tileHeight; // we use square tiles anyway

	const rapidjson::Value &layers = document["layers"];
	for (rapidjson::SizeType i = 0; i < layers.Size(); i++) { // Uses SizeType instead of size_t
		MapLayer layer;
		layer.id = layers[i]["id"].GetInt();
		layer.height = layers[i]["height"].GetInt();
		layer.width = layers[i]["width"].GetInt();
		layer.name = layers[i]["name"].GetString();
		layer.collision = layers[i]["collision"].GetBool();

		if (layers[i].HasMember("targetMap")) {
			layer.targetMap = layers[i]["targetMap"].GetString();
			layer.playerStart = Vector2D(layers[i]["playerStartX"].GetInt(), layers[i]["playerStartY"].GetInt());
		}

		const rapidjson::Value &tiles = layers[i]["data"];
		for (rapidjson::SizeType j = 0; j < tiles.Size(); j++) { // Uses SizeType instead of size_t
			layer.tiles.push_back(tiles[j].GetInt());
		}
		setting.layers.push_back(layer);
	}

	const rapidjson::Value &tilesets = document["tilesets"];
	for (rapidjson::SizeType i = 0; i < tilesets.Size(); i++) { // Uses SizeType instead of size_t
		TileSet tileset;
		tileset.firstId = tilesets[i]["firstgid"].GetInt();
		tileset.spriteId = tilesets[i]["spriteId"].GetString();
		tileset.columns = tilesets[i]["columns"].GetInt();
		tileset.imageName = tilesets[i]["image"].GetString();

		RpgPlayState::assets->AddTexture(tileset.spriteId, "../rpg/assets/map_sprites/" + tileset.imageName);
		setting.tilesets.push_back(tileset);
	}

	height = setting.height * setting.tileHeight;
	width = setting.width * setting.tileWidth;
	scale = setting.mapScale;
}

Map::~Map() {}

void Map::LoadMap()
{
	int counter = 0;
	for (int layerNumber = 0; layerNumber < setting.layers.size(); layerNumber++) {
		int tileIndex = 0;
		for (int y = 0; y < setting.height; y++) {
			for (int x = 0; x < setting.width; x++) {

				int tileNumber = setting.layers[layerNumber].tiles[tileIndex];
				if (tileNumber >= 1) {
					TileSet tileset = FindTileset(tileNumber);
					tileNumber -= tileset.firstId; // normalize number

					int xpos = ((tileNumber % tileset.columns) + 0) * setting.tileWidth;
					int ypos = ((tileNumber / tileset.columns) + 0) * setting.tileHeight;

					std::string mapTest;
					Vector2D pPos;
					if (x == 30 && y == 25) {
						mapTest = "../rpg/assets/map/shop.json";
						pPos = Vector2D(4, 7);
					}

					if (setting.layers[layerNumber].targetMap != "") {
						mapTest = "../rpg/assets/map/" + setting.layers[layerNumber].targetMap;
						pPos = setting.layers[layerNumber].playerStart;
					}

					RpgPlayState::assets->CreateMapTile(
					    xpos, ypos, x * setting.ScaledWidth(), y * setting.ScaledHeight(), setting.tileSize,
					    setting.mapScale, setting.layers[layerNumber].collision, tileset.spriteId,
					    SpriteSheet(tileset.columns, setting.tileWidth, setting.tileHeight, 0, 0), mapTest, pPos);
					counter++;
				}

				tileIndex++;
			}
		}
	}
}
