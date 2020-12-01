#pragma once

#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "ecs/ecs.hpp"
#include <map>
#include <string>

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	//texture management
	void AddTexture(std::string id, std::string path);
	SDL_Texture* GetTexture(std::string id);

	

private:

	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;	
};