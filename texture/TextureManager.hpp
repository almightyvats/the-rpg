#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
class TextureManager {

  public:
	static SDL_Texture *LoadTexture(const std::string fileName);
	static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest);
};