#include "TextureManager.hpp"
#include "RpgGame.hpp"

SDL_Texture *TextureManager::LoadTexture(std::string texture)
{
	SDL_Surface *tempSurface = IMG_Load(texture.c_str());
	SDL_Texture *tex = SDL_CreateTextureFromSurface(RpgGame::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture *tex, const SDL_Rect *src, const SDL_Rect *dest, SDL_RendererFlip flip, Uint8 alpha)
{
	SDL_SetTextureAlphaMod(tex, alpha);
	SDL_RenderCopyEx(RpgGame::renderer, tex, src, dest, 0, NULL, flip);
}