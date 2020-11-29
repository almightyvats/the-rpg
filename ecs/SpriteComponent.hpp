#pragma once

#include "Components.hpp"
#include "TextureManager.hpp"
#include "animation.hpp"
#include "RpgGame.hpp"
#include <map>

class SpriteComponent : public Component {
  private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

  public:
	int animIndex = 0;

	std::map<const char *, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(const std::string path) { setTex(path); }
	SpriteComponent(const std::string path, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle_down = Animation(0, 1, 100);
		Animation idle_up = Animation(1, 1, 100);
		Animation idle_left = Animation(2, 1, 100);
		Animation idle_right = Animation(3, 1, 100);

		Animation walk_down = Animation(0, 7, 100);
		Animation walk_up = Animation(1, 7, 100);
		Animation walk_left = Animation(2, 7, 100);
		Animation walk_right = Animation(3, 7, 100);

		animations.emplace("idle_down", idle_down);
		animations.emplace("idle_up", idle_up);
		animations.emplace("idle_left", idle_left);
		animations.emplace("idle_right", idle_right);

		animations.emplace("walk_down", walk_down);
		animations.emplace("walk_up", walk_up);
		animations.emplace("walk_left", walk_left);
		animations.emplace("walk_right", walk_right);

		play("idle_down");

		setTex(path);
	}

	~SpriteComponent() { SDL_DestroyTexture(texture); }

	void setTex(std::string path) { texture = TextureManager::LoadTexture(path); }

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}
	void update() override
	{
		if (animated) {
			srcRect.x = 2 * srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - RpgGame::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - RpgGame::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}
	void draw() override { TextureManager::Draw(texture, srcRect, destRect, spriteFlip); }

	void play(const char *animName)
	{

		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};
