#pragma once

#include "Components.hpp"
#include "rpg/Vector2D.hpp"

enum InventoryAction : std::size_t { actionNone, actionDelete };

struct InventoryComponent : public Component {

  private:
	TransformComponent *transform;

  public:
	int pocketNumber;
	bool isInDragAndDrop;
	InventoryAction action;

	InventoryComponent() { this->action = actionNone; };
	InventoryComponent(int pocketNumber)
	{
		this->pocketNumber = pocketNumber;
		this->action = actionNone;
	};

	void init() override
	{
		isInDragAndDrop = false;
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}
	void update() override {};
};