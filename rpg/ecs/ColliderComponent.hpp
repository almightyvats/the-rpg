#pragma once

#include "Components.hpp"
#include <string>

class ColliderComponent : public Component {
  public:
	SDL_Rect collider;
	std::string tag;

	TransformComponent *transform;

	ColliderComponent(const std::string tag) { this->tag = tag; }

	ColliderComponent(const std::string tag, int srcX, int srcY, int xpos, int ypos, int tsize, int tscale,
	                  const std::string id)
	{
		this->tag = tag;

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
	}

	void update() override
	{
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.h = transform->height * transform->scale;
		collider.w = transform->width * transform->scale;
	}
};