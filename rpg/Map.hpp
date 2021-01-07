#pragma once
#include "Vector2D.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

struct MapLayer {
	int id;
	int height;
	int width;
	bool collision = false;
	std::string name;
	std::vector<int> tiles;
	std::string targetMap;
	Vector2D playerStart;
};

struct TileSet {
	int firstId;
	std::string spriteId;
	int columns;
	std::string imageName;
};

struct Npc {
	int xPos;
	int yPos;
	std::string spriteId;
};

struct Enemy {
	int xPos;
	int yPos;
	std::string spriteId;
};

struct MapSetting {
	int height;
	int width;
	int tileHeight;
	int tileWidth;
	int mapScale;
	int tileSize;
	std::vector<MapLayer> layers;
	std::vector<TileSet> tilesets;
	std::vector<Npc> npcs;
	std::vector<Enemy> enemies;

	int ScaledHeight() { return tileHeight * mapScale; }
	int ScaledWidth() { return tileWidth * mapScale; }
};

class Map {
  public:
	Map(std::string path, int mapScale);
	~Map();

	void LoadMap();

	int height;
	int width;
	int scale;
	MapSetting setting;

  private:
	std::string mapFilePath;

	TileSet FindTileset(int tileNumber)
	{
		TileSet result;
		result.firstId = 0; // initial condition
		// we need the tileset with the maximum firstId lower or equal than the tile number
		for (auto &tileset : setting.tilesets) {
			if (tileset.firstId <= tileNumber) {
				if (tileset.firstId > result.firstId) {
					result = tileset;
				}
			}
		}
		return result;
	}
};