#pragma once

#include "Components.hpp"
#include "rpg/Vector2D.hpp"

struct TransformComponent : public Component {
  public:
	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 3;

	TransformComponent() { position.Zero(); }

	TransformComponent(int scale)
	{
		position.Zero();
		this->scale = scale;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int height, int width, int scale)
	{
		position.x = x;
		position.y = y;
		this->height = height;
		this->width = width;
		this->scale = scale;
	}

	void init() override { velocity.Zero(); }
	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};