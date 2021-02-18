#pragma once

#include "RpgGame.hpp"

class GameObject {
  public:
	GameObject(std::string textureSheet, int x, int y);
	~GameObject();

	void Update();
	void Render();

  private:
	int xpos;
	int ypos;

	SDL_Texture *objTexture;
	SDL_Rect scrRect, destRect;
	SDL_Renderer *renderer;
};