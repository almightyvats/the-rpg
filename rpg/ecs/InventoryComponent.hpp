#pragma once

#include "../combat/Equipment.hpp"
#include "Components.hpp"
#include "rpg/Vector2D.hpp"

enum InventoryAction : std::size_t { actionNone, actionDelete };

struct InventoryComponent : public Component {

  private:
	TransformComponent *transform;

  public:
	int pocketNumber = -1;
	bool isInDragAndDrop = actionNone;
	InventoryAction action;
	Equipment equip;

	InventoryComponent() = default;
	// InventoryComponent(int pocketNumber) { this->pocketNumber = pocketNumber; }
	InventoryComponent(int pocketNumber) { this->pocketNumber = pocketNumber; }
	~InventoryComponent() {}

	void init() override
	{
		isInDragAndDrop = false;
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}
	void update() override{};
};