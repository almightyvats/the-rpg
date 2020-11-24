#pragma once

#include "Components.hpp"
#include "TextureManager.hpp"

class SpriteComponent : public Component {
  private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

  public:
	SpriteComponent() = default;
	SpriteComponent(const std::string path) { setTex(path); }
	SpriteComponent(const std::string path, int nFrames, int mSpeed)
	{
		animated = true;
		frames = nFrames;
		speed = mSpeed;
		setTex(path);
	}

	~SpriteComponent() { SDL_DestroyTexture(texture); }

	void setTex(std::string path) { texture = TextureManager::LoadTexture(path); }

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width * transform->scale;
		srcRect.h = transform->height * transform->scale;
		destRect.w = destRect.h = 32;
	}
	void update() override
	{
		destRect.x = (int)transform->position.x;
		destRect.y = (int)transform->position.y;
	}
	void draw() override { TextureManager::Draw(texture, srcRect, destRect); }
};
