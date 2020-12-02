#pragma once

#include "../Vector2D.hpp"
#include "Components.hpp"
#include "ecs.hpp"

class ProjectileComponent : public Component {
  public:
	ProjectileComponent(int range, int speed, Vector2D velocity)
	{
		this->range = range;
		this->speed = speed;
		this->velocity = velocity;
	}
	~ProjectileComponent(){};

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}
	void update() override
	{
		distance += speed;

		if (distance > range) {
			std::cout << "Out of range" << std::endl;
			entity->destroy();
		} else if ((transform->position.x > RpgGame::camera.x + RpgGame::camera.w)
		           || (transform->position.y > RpgGame::camera.y + RpgGame::camera.h)
		           || (transform->position.x < RpgGame::camera.x) || (transform->position.y < RpgGame::camera.y)) {
			std::cout << "Out of bounds" << std::endl;
			entity->destroy();
		}
	}

  private:
	TransformComponent *transform;
	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};