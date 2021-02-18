#pragma once

#include "Vector2D.hpp"
#include "RpgGame.hpp"

struct MapLayer {
	int id;
	int height;
	int width;
	bool collision = false;
	std::string name;
	std::vector<int> tiles;
	std::string targetMap;
	Vector2D playerStart;
	int minLvl;
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
	std::string name;
};

struct Enemy {
	int xPos;
	int yPos;
	std::string spriteId;
};

struct Projectile {
	int xPos;
	int yPos;
	int xVel;
	int yVel;
	int range;
	int speed;
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
	std::vector<Projectile> projectiles;

	int ScaledHeight() { return tileHeight * mapScale; }
	int ScaledWidth() { return tileWidth * mapScale; }
};

class Map {
  public:
	Map(std::string path, int mapScale);
	~Map();

	void LoadMap();
	void ClearMap();

	int height;
	int width;
	int scale;
	MapSetting setting;
	std::string mapFilePath;

  private:
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