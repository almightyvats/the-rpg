#pragma once

#include "Components.hpp"
#include "rpg/Vector2D.hpp"

struct DoorComponent : public Component {
  public:
	DoorComponent(std::string targetMap, Vector2D playerStart)
	{
		this->targetMap = targetMap;
		this->playerStart = playerStart;
	}
	~DoorComponent() {}

	std::string targetMap;
	Vector2D playerStart;
};