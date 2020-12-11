#pragma once

#include "../AssetManager.hpp"
#include "../SpriteSheet.hpp"
#include "Components.hpp"
#include "animation.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/TextureManager.hpp"
#include <map>

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

	std::string id;

	std::map<std::string, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const std::string id, SpriteSheet spriteSheet)
	{
		this->id = id;
		this->spriteSheet = spriteSheet;
		setTex(id);
		srcRect.x = srcRect.y = 0;
		srcRect.w = spriteSheet.spriteWidth;
		srcRect.h = spriteSheet.spriteHeight;
	}
	SpriteComponent(const std::string id, int srcX, int srcY, SpriteSheet spriteSheet)
	{
		this->id = id;
		this->spriteSheet = spriteSheet;
		setTex(id);
		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = spriteSheet.spriteWidth;
		srcRect.h = spriteSheet.spriteHeight;
	}
	~SpriteComponent() {}

	void addAnimation(std::string tag, Animation animation)
	{
		animations.emplace(tag, animation);
		animated = true;
	}

	void setTex(std::string id) { texture = RpgGame::assets->GetTexture(id); }

	void init() override { transform = &entity->getComponent<TransformComponent>(); }
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
	void draw() override
	{
		if (id == "everything") {
			std::cout << srcRect.x << " " << srcRect.y << " " << srcRect.w << " " << srcRect.h << std::endl;
		}
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void defaultAnimation(std::string tag) { play(tag); }

	void play(std::string tag)
	{
		frames = animations[tag].frames;
		animationIndex = animations[tag].index;
		speed = animations[tag].speed;
	}
};
