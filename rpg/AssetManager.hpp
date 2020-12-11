#pragma once

#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ecs/ecs.hpp"
#include "SpriteSheet.hpp"
#include <map>
#include <string>

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	//game objects
	void CreateProjectile(Vector2D position,Vector2D velocity, int range, int speed, std::string id);

	void CreateMapTile(int srcX, int srcY, int destX, int destY, int tsize, int tscale, bool withCollision, std::string id, SpriteSheet spriteSheet);

	//texture management
	void AddTexture(std::string id, std::string path);
	SDL_Texture* GetTexture(std::string id);

	

private:

	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;	
};