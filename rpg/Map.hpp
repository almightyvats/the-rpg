#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

struct MapLayer {
	int id;
	int height;
	int width;
	bool collision;
	std::string name;
	std::vector<int> tiles;
};

struct TileSet {
	int firstId;
	std::string spriteId;
	int columns;
	std::string imageName;
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

  private:
	std::string mapFilePath;
	MapSetting setting;

	TileSet FindTileset(int tileNumber)
	{
		TileSet result;
		result.firstId = 0; //initial condition
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