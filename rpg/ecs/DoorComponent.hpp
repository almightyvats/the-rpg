#pragma once

#include "Components.hpp"
#include "rpg/Vector2D.hpp"

struct DoorComponent : public Component {
  public:
	DoorComponent(std::string targetMap, Vector2D playerStart, int minLvl)
	{
		this->targetMap = targetMap;
		this->playerStart = playerStart;
		this->minLvl = minLvl;
	}
	~DoorComponent() {}

	std::string targetMap;
	Vector2D playerStart;
	int minLvl;
};