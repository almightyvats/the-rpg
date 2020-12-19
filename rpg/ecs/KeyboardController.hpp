#pragma once

#include "Components.hpp"
#include "ecs.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/states/RpgPlayState.hpp"

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
		if (RpgPlayState::event.type == SDL_KEYDOWN) {
			switch (RpgPlayState::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->play("walk_up");
				break;
			case SDLK_a:
				sprite->spriteFlip = SDL_FLIP_NONE;
				transform->velocity.x = -1;
				sprite->play("walk_left");
				break;
			case SDLK_s:
				sprite->spriteFlip = SDL_FLIP_NONE;
				transform->velocity.y = 1;
				sprite->play("walk_down");
				break;
			case SDLK_d:
				sprite->spriteFlip = SDL_FLIP_NONE;
				transform->velocity.x = 1;
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				sprite->play("walk_left");
				break;
			default:
				break;
			}
		}

		if (RpgPlayState::event.type == SDL_KEYUP) {
			switch (RpgPlayState::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->play("idle_up");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->play("idle_left");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->play("idle_down");
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->play("idle_left");//is flipped
				break;
			case SDLK_ESCAPE:
				RpgGame::isRunning = false;
				break;
			default:
				break;
			}
		}
	}

  private:
};
