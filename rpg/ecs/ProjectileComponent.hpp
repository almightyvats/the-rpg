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
	}

  private:
	TransformComponent *transform;
	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};