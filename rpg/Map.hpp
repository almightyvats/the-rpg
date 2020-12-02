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
	std::string name;
	std::vector<int> tiles;
};

struct MapSetting {
	int height;
	int width;
	int tileHeight;
	int tileWidth;
	int mapScale;
	int tileSize;
	std::vector<MapLayer> layers;

	int ScaledHeight() { return tileHeight * mapScale; }
	int ScaledWidth() { return tileWidth * mapScale; }
};

class Map {
  public:
	Map(std::string path, int mapScale);
	~Map();

	void LoadMap(std::string id);
	void AddTile(int srcX, int srcY, int xPos, int yPos, int tsize, int tscale, bool withCollision, std::string id);

	int height;
	int width;
	int scale;

  private:
	std::string mapFilePath;
	MapSetting setting;
};