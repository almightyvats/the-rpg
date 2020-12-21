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

		if (layers[i].HasMember("properties")) {
			const rapidjson::Value &properties = layers[i]["properties"];
			for (rapidjson::SizeType j = 0; j < properties.Size(); j++) { // Uses SizeType instead of size_t^
				std::string propName = properties[j]["name"].GetString();
				if (propName.compare("collision") == 0) {
					layer.collision = properties[j]["value"].GetBool();
				} else if (propName.compare("targetMap") == 0) {
					std::string strVal = properties[j]["value"].GetString();
					layer.targetMap = "../rpg/assets/map/" + strVal + ".json";
				} else if (propName.compare("playerStartX") == 0) {
					layer.playerStart.x = properties[j]["value"].GetInt();
				} else if (propName.compare("playerStartY") == 0) {
					layer.playerStart.y = properties[j]["value"].GetInt();
				}
			}
		}

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

	if (document.HasMember("npcs")) {
		const rapidjson::Value &npcs = document["npcs"];
		for (rapidjson::SizeType n = 0; n < npcs.Size(); n++) { // Uses SizeType instead of size_t^
			setting.npcs.emplace_back<Npc>(
			    {npcs[n]["x"].GetInt(), npcs[n]["y"].GetInt(), npcs[n]["model"].GetString()});
		}
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

					RpgPlayState::assets->CreateMapTile(
					    xpos, ypos, x * setting.ScaledWidth(), y * setting.ScaledHeight(), setting.tileSize,
					    setting.mapScale, setting.layers[layerNumber].collision, tileset.spriteId,
					    SpriteSheet(tileset.columns, setting.tileWidth, setting.tileHeight, 0, 0),
					    setting.layers[layerNumber].targetMap, setting.layers[layerNumber].playerStart);
					counter++;
				}

				tileIndex++;
			}
		}
	}
	for (auto npc : setting.npcs) {
		RpgPlayState::assets->CreateNpc(Vector2D(npc.xPos, npc.yPos), setting.tileSize, scale, npc.spriteId);
	}
}
