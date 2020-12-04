#pragma once

#include "../AssetManager.hpp"
#include "Components.hpp"
#include "animation.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/TextureManager.hpp"
#include "rpg/states/RpgPlayState.hpp"
#include <map>

class SpriteSheet {
  public:
	SpriteSheet() {}
	SpriteSheet(int rows, int spriteWidth, int spriteHeight, int spriteGapX, int spriteGapY)
	{
		this->rows = rows;
		this->spriteWidth = spriteWidth;
		this->spriteWidth = spriteWidth;
		this->spriteHeight = spriteHeight;
		this->spriteGapX = spriteGapX;
		this->spriteGapY = spriteGapY;
	};
	~SpriteSheet(){};

	int rows = 0;
	int spriteWidth = 32;
	int spriteHeight = 32;
	int spriteGapX = 0;
	int spriteGapY = 0;
};

class SpriteComponent : public Component {
  private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;
	SpriteSheet spriteSheet;

	bool animated = false;
	int frames = 0;
	int speed = 100;

  public:
	int animationIndex = 0;

	std::map<std::string, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const std::string id, SpriteSheet spriteSheet)
	{
		this->spriteSheet = spriteSheet;
		setTex(id);
	}
	~SpriteComponent() {}

	void addAnimation(std::string tag, Animation animation)
	{
		animations.emplace(tag, animation);
		animated = true;
	}

	void setTex(std::string id) { texture = RpgPlayState::assets->GetTexture(id); }

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = spriteSheet.spriteWidth;
		srcRect.h = spriteSheet.spriteHeight;
	}
	void update() override
	{
		if (animated) {
			srcRect.x = (spriteSheet.spriteGapX + srcRect.w) * static_cast<int>((SDL_GetTicks() / speed) % frames);
			srcRect.y = (spriteSheet.spriteGapY + srcRect.h) * animationIndex;
		}

		destRect.x = static_cast<int>(transform->position.x) - RpgGame::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - RpgGame::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}
	void draw() override { TextureManager::Draw(texture, srcRect, destRect, spriteFlip); }

	void defaultAnimation(std::string tag) { play(tag); }

	void play(std::string tag)
	{
		frames = animations[tag].frames;
		animationIndex = animations[tag].index;
		speed = animations[tag].speed;
	}
};
