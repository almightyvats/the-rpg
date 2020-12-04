#pragma once

#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"
#include "ecs.hpp"
#include <SDL.h>

class TileComponent : public Component {
  public:
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;

	TileComponent() = default;

	~TileComponent() { SDL_DestroyTexture(texture); }

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, const std::string id)
	{
		texture = RpgPlayState::assets->GetTexture(id);
		
		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;
		position.x = static_cast<float>(xpos);
		position.y = static_cast<float>(ypos);
		destRect.w = destRect.h = tsize * tscale;
	}
	void update() override
	{
		destRect.x = static_cast<int>(position.x - RpgGame::camera.x);
		destRect.y = static_cast<int>(position.y - RpgGame::camera.y);
	}
	
	void draw() override { TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE); }
};
