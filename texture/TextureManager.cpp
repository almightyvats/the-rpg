#include "TextureManager.hpp"
#include "RpgGame.hpp"

SDL_Texture *TextureManager::LoadTexture(std::string texture)
{
	SDL_Surface *tempSurface = IMG_Load(texture.c_str());
	SDL_Texture *tex = SDL_CreateTextureFromSurface(RpgGame::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(RpgGame::renderer, tex, &src, &dest);
}