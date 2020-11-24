#pragma once

#include "Components.hpp"
#include "RpgGame.hpp"
#include "ecs.hpp"

class KeyboardController : public Component {
  public:
	TransformComponent *transform;
	SpriteComponent *sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}
	void update() override
	{
		if (RpgGame::event.type == SDL_KEYDOWN) {
			switch (RpgGame::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->play("walk_up");
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->play("walk_left");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				sprite->play("walk_down");
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->play("walk_right");
				break;
			default:
				break;
			}
		}

		if (RpgGame::event.type == SDL_KEYUP) {
			switch (RpgGame::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->play("idle_up");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->play("idle_left");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->play("idle_down");
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->play("idle_right");
				break;
			default:
				break;
			}
		}
	}

  private:
};
