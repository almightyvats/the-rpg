#pragma once

#include "SpriteSheet.hpp"
#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "combat/Equipment.hpp"
#include "ecs/ecs.hpp"
#include <SDL_ttf.h>
#include <map>
#include <string>

class AssetManager {
  public:
	AssetManager(Manager *man);
	~AssetManager();

	// game objects
	void CreateNpc(Vector2D position, int tileSize, int mapScale, std::string id, State state);
	void CreateEnemy(Vector2D position, int tileSize, int mapScale, std::string id, State state);
	void CreateProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string id, State state);
	void CreateMapTile(int srcX, int srcY, int destX, int destY, int tsize, int tscale, bool withCollision,
	                   std::string id, SpriteSheet spriteSheet, std::string map, Vector2D pStart, State state);
	void CreateCombatant(Vector2D position, std::string sprite_name, bool player_team, State state);
	void CreateInventoryItem(Equipment equip);

	// texture management
	void AddTexture(std::string id, std::string path);
	SDL_Texture *GetTexture(std::string id);

	void AddFont(const std::string &id, const std::string &path, const int &fontSize);
	TTF_Font *GetFont(const std::string &id);

  private:
	Manager *manager;
	std::map<std::string, SDL_Texture *> m_textures;
	std::map<std::string, TTF_Font *> m_fonts;
};