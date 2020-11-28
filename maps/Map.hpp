#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

class Map {
  public:
	Map();
	~Map();

	static void LoadMap(std::string path);
	
  private:
	
};