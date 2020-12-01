#pragma once
#include "Components.hpp"
#include <string>

class ColliderComponent : public Component {
  public:
	SDL_Rect collider;
	std::string tag;
	std::string path;

	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	TransformComponent *transform;

	ColliderComponent(const std::string tag) { this->tag = tag; }

	ColliderComponent(const std::string tag, int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, const std::string path)
	{
		this->tag = tag;
		this->path = path;

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		collider.x = xpos;
		collider.y = ypos;
		collider.w = collider.h = tsize * tscale;		
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		texture = TextureManager::LoadTexture(path);
		
		destRect = {collider.x, collider.y, collider.w, collider.h};
	}

	void update() override
	{
		if (tag != "Trees") {
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.h = transform->height * transform->scale;
			collider.w = transform->width * transform->scale;
		}

		destRect.x = collider.x - RpgGame::camera.x;
		destRect.y = collider.y - RpgGame::camera.y;
	}

	void draw() override { TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE); }
};