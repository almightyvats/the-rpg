#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
class TextureManager {

  public:
	static SDL_Texture *LoadTexture(const std::string fileName);
	static void Draw(SDL_Texture *tex, const SDL_Rect *src, const SDL_Rect *dest, SDL_RendererFlip flip,
	                 Uint8 alpha = SDL_ALPHA_OPAQUE);
};