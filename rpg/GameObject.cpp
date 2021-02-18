#include "GameObject.hpp"
#include "RpgGame.hpp"
#include "TextureManager.hpp"

GameObject::GameObject(std::string textureSheet, int x, int y)
{
	objTexture = TextureManager::LoadTexture(textureSheet);

	xpos = x;
	ypos = y;
}

void GameObject::Update()
{
	scrRect.h = 32;
	scrRect.w = 32;
	scrRect.x = 0;
	scrRect.y = 0;

	destRect.x = xpos * 2;
	destRect.y = ypos * 2;
	destRect.w = scrRect.w;
	destRect.h = scrRect.h;
}

void GameObject::Render()
{
	SDL_RenderCopy(RpgGame::renderer, objTexture, NULL, &destRect);
}