#pragma once

#include "Components.hpp"
#include "rpg/Vector2D.hpp"

struct InventoryComponent : public Component {

  private:
	TransformComponent *transform;

	int xPockets = 7;
	int yPockets = 8;
	int scale = 2;
	int resolution = 32;
	int border = 5;

	Vector2D getPocketCoords(const int pocketNumber)
	{
		int x = pocketNumber % xPockets;
		int y = floor(pocketNumber / xPockets);

		return Vector2D(border * (x + 1) + (x * resolution * scale), border * (y + 1) + (y * resolution * scale));
	}

	Vector2D getItemCoords(const int pocketNumber)
	{
		auto pocket = getPocketCoords(pocketNumber);
		pocket.x += resolution / 2;
		pocket.y += resolution / 2;
		return pocket;
	}

  public:
	int pocketNumber;
	bool isInDragAndDrop;
	const bool isMovable;

	InventoryComponent(bool movable) : isMovable(movable){};
	InventoryComponent(int pocketNumber, bool movable) : isMovable(movable) { this->pocketNumber = pocketNumber; };

	void init() override
	{
		isInDragAndDrop = false;
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}
	void update()
	{
		if (!isInDragAndDrop && isMovable) {
			auto coords = getItemCoords(pocketNumber);
			transform->position.x = coords.x;
			transform->position.y = coords.y;
		}
	}
};