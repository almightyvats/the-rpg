
#include "RpgInventoryState.hpp"
#include "RpgStates.hpp"
#include "rpg/AssetManager.hpp"
#include "rpg/RpgSoundManager.hpp"
#include "rpg/ecs/Components.hpp"
#include "rpg/inventory/GuiHelper.hpp"
#include <set>

SDL_Event RpgInventoryState::m_event;
GuiHelper *guiHelper = new GuiHelper("../rpg/assets/gui/uipackSpace_sheet.json");

extern Manager manager;
const State m_state = stateInventory;

SDL_Texture *background;

SDL_Color orange = {255, 165, 0};
SDL_Color black = {255, 255, 255};
bool drawInfo = false;
LabelItem labelItemName;
LabelItem labelItemInfo;

int yStart = 5;
int xPockets = 10;
int yPockets = 4;
int scale = 2;
int resolution = 32;
int border = 5;

Vector2D getPocketCoords(const int pocketNumber)
{
	int x = pocketNumber % xPockets;
	int y = floor(pocketNumber / xPockets) + yStart;

	return Vector2D(border * (x + 1) + (x * resolution * scale), border * (y + 1) + (y * resolution * scale));
}

void createInventorySlots()
{
	auto spriteInfo = guiHelper->getSpriteInfo("glassPanel.png");
	for (int i = 0; i < 40; i++) {
		auto coords = getPocketCoords(i);
		auto &item(manager.addEntity(m_state));
		item.addComponent<TransformComponent>(coords.x, coords.y, resolution, resolution, scale);
		item.addComponent<InventoryComponent>(i);
		item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
		                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
		item.addGroup(RpgPlayState::groupGui);
	}
}

void createDeleteItemSlot()
{
	// delete-item pocket
	{
		auto spriteInfo = guiHelper->getSpriteInfo("glassPanel_corners.png");
		auto coords = getPocketCoords(49);
		auto &item(manager.addEntity(m_state));
		item.addComponent<TransformComponent>(coords.x, coords.y, resolution, resolution, scale);
		auto &comp = item.addComponent<InventoryComponent>(49);
		comp.action = actionDelete;
		item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
		                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
		item.addGroup(RpgPlayState::groupGui);
	}
	// left
	{
		auto spriteInfo = guiHelper->getSpriteInfo("barHorizontal_red_left.png");
		auto coords = getPocketCoords(49);
		auto &item(manager.addEntity(m_state));
		item.addComponent<TransformComponent>(coords.x, coords.y, resolution, 3, scale);
		item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
		                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
		item.addGroup(RpgPlayState::groupGui);
	}
	// right
	{
		auto spriteInfo = guiHelper->getSpriteInfo("barHorizontal_red_right.png");
		auto coords = getPocketCoords(49);
		coords.x += (resolution - 3) * scale;
		auto &item(manager.addEntity(m_state));
		item.addComponent<TransformComponent>(coords.x, coords.y, resolution, 3, scale);
		item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
		                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
		item.addGroup(RpgPlayState::groupGui);
	}
	// top
	{
		auto spriteInfo = guiHelper->getSpriteInfo("barVertical_red_top.png");
		auto coords = getPocketCoords(49);
		auto &item(manager.addEntity(m_state));
		item.addComponent<TransformComponent>(coords.x, coords.y, 3, resolution, scale);
		item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
		                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
		item.addGroup(RpgPlayState::groupGui);
	}
	// bottom
	{
		auto spriteInfo = guiHelper->getSpriteInfo("barVertical_red_bottom.png");
		auto coords = getPocketCoords(49);
		coords.y += (resolution - 3) * scale;
		auto &item(manager.addEntity(m_state));
		item.addComponent<TransformComponent>(coords.x, coords.y, 3, resolution, scale);
		item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
		                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
		item.addGroup(RpgPlayState::groupGui);
	}
}

void createSwordItemSlot()
{
	auto spriteInfo = guiHelper->getSpriteInfo("glassPanel_cornerTL.png");
	auto coords = getPocketCoords(9);
	coords.x += (32 + border) * scale;
	auto &item(manager.addEntity(m_state));
	item.addComponent<TransformComponent>(coords.x, coords.y, resolution, resolution, scale + 1);
	item.addComponent<InventoryComponent>(100);
	item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
	                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
	item.addGroup(RpgPlayState::groupGui);
}

void createBowItemSlot()
{
	auto spriteInfo = guiHelper->getSpriteInfo("glassPanel_cornerTL.png");
	auto coords = getPocketCoords(9);
	coords.x += 3 * (32 + border) * scale;
	auto &item(manager.addEntity(m_state));
	item.addComponent<TransformComponent>(coords.x, coords.y, resolution, resolution, scale + 1);
	item.addComponent<InventoryComponent>(200);
	item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
	                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
	item.addGroup(RpgPlayState::groupGui);
}

void createShieldItemSlot()
{
	auto spriteInfo = guiHelper->getSpriteInfo("glassPanel_cornerTL.png");
	auto coords = getPocketCoords(29);
	coords.x += (32 + border) * scale;
	auto &item(manager.addEntity(m_state));
	item.addComponent<TransformComponent>(coords.x, coords.y, resolution, resolution, scale + 1);
	item.addComponent<InventoryComponent>(300);
	item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
	                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
	item.addGroup(RpgPlayState::groupGui);
}

void createHealItemSlot()
{
	auto spriteInfo = guiHelper->getSpriteInfo("glassPanel_cornerTL.png");
	auto coords = getPocketCoords(29);
	coords.x += 3 * (32 + border) * scale;
	auto &item(manager.addEntity(m_state));
	item.addComponent<TransformComponent>(coords.x, coords.y, resolution, resolution, scale + 1);
	item.addComponent<InventoryComponent>(400);
	item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
	                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
	item.addGroup(RpgPlayState::groupGui);
}

RpgInventoryState::RpgInventoryState()
{
	RpgGame::assets->AddFont("Ancient", "../rpg/assets/font/ancient.ttf", 20);
	RpgGame::assets->AddTexture("gui", "../rpg/assets/gui/uipackSpace_sheet.png");

	// Inventory slots
	createInventorySlots();
	// Delete item
	createDeleteItemSlot();
	// Sword
	createSwordItemSlot();
	// Bow
	createBowItemSlot();
	// Shield
	createShieldItemSlot();
	// Shield
	createHealItemSlot();

	// TODO: testdata
	int item_level = 1;
	Equipment sword = Equipment("testsword", EquipmentType::sword, item_level, -(int)(0.5 * item_level),
	                            (int)(0.5 * item_level), 1, 0, 5, 0);

	RpgGame::assets->AddTexture("icons", "../rpg/assets/icons/Icon Pack_3.png");
	RpgGame::assets->CreateInventoryItem(0, 0, "icons", sword, m_state);
	RpgGame::assets->CreateInventoryItem(0, 4, "icons", sword, m_state);
	RpgGame::assets->CreateInventoryItem(5, 0, "icons", sword, m_state);
	RpgGame::assets->CreateInventoryItem(7, 7, "icons", sword, m_state);
	RpgGame::assets->CreateInventoryItem(9, 4, "icons", sword, m_state);
	RpgGame::assets->CreateInventoryItem(9, 8, "icons", sword, m_state);

	background = TextureManager::LoadTexture("../rpg/assets/game_background_4.png");

	labelItemName = std::make_shared<RpgLabel>(50, 10, "ItemName", "Ancient", orange);
	labelItemInfo = std::make_shared<RpgLabel>(50, 10, "ItemInfo", "Ancient", black);
}

RpgInventoryState::~RpgInventoryState() {}

auto &guiElements(manager.getGroup(RpgPlayState::groupGui));
auto &invItems(manager.getGroup(RpgPlayState::groupItems));

void RpgInventoryState::Pause()
{
	RpgSoundManager::pauseMusic();
}

void RpgInventoryState::Resume()
{
	RpgSoundManager::resumeMusic("PLAY");
}

bool IsValidDropTarget(SDL_Point mousePos, int &target, InventoryAction &action)
{
	for (auto &i : guiElements) {
		if (i->hasComponent<SpriteComponent>() && i->hasComponent<InventoryComponent>()) {
			auto rect = i->getComponent<SpriteComponent>().getDestRect();
			if (SDL_PointInRect(&mousePos, rect)) {
				target = i->getComponent<InventoryComponent>().pocketNumber;
				action = i->getComponent<InventoryComponent>().action;
				return true;
			}
		}
	}
	return false;
}

// if another Item is at target position - swap positions
void TrySwapPocket(int sourcePocket, int targetPocket)
{
	for (auto &i : invItems) {
		if (i->hasComponent<InventoryComponent>()) {
			auto &inv = i->getComponent<InventoryComponent>();
			if (inv.pocketNumber == targetPocket) {
				inv.pocketNumber = sourcePocket;
				break;
			}
		}
	}
}

bool tryShowHint(SDL_Point &mousePos)
{
	for (auto &i : invItems) {
		if (i->hasComponent<SpriteComponent>()) {
			auto rect = i->getComponent<SpriteComponent>().getDestRect();
			if (SDL_PointInRect(&mousePos, rect) && i->hasComponent<InventoryComponent>()) {
				labelItemName->setLabelPos(Vector2D(rect->x + 2 * resolution, rect->y));
				labelItemName->setLabelText("Ancient", i->getComponent<InventoryComponent>().equip.name());
				labelItemInfo->setLabelPos(Vector2D(rect->x + 2 * resolution, rect->y + resolution));
				labelItemInfo->setLabelText("Ancient", i->getComponent<InventoryComponent>().equip.item_info());
				return true;
			}
		}
	}
	return false;
}

Entity *dragAndDropItem = NULL;
bool buttonClicked = false;
bool leftMouseButtonDown = false;

SDL_Point clickOffset;

SDL_Point mousePos;
void RpgInventoryState::HandleEvents(RpgGame *rpgGame)
{

	SDL_PollEvent(&m_event);
	switch (m_event.type) {
	case SDL_QUIT:
		rpgGame->quitGame();
		break;
	case SDL_KEYDOWN:
		switch (m_event.key.keysym.sym) {
		case SDLK_ESCAPE:
		case SDLK_i:
			rpgGame->changeState(RpgPlayState::Instance());
			break;
		}
		break;
	case SDL_MOUSEMOTION:
		mousePos = {m_event.motion.x, m_event.motion.y};

		drawInfo = tryShowHint(mousePos);
		if (leftMouseButtonDown && dragAndDropItem != NULL) {
			dragAndDropItem->getComponent<TransformComponent>().position.x = mousePos.x - clickOffset.x;
			dragAndDropItem->getComponent<TransformComponent>().position.y = mousePos.y - clickOffset.y;
		}

		break;
	case SDL_MOUSEBUTTONUP:
		if (leftMouseButtonDown && m_event.button.button == SDL_BUTTON_LEFT) {
			leftMouseButtonDown = false;
			if (dragAndDropItem != NULL) {
				int sourcePocket = dragAndDropItem->getComponent<InventoryComponent>().pocketNumber;
				int targetPocket;
				InventoryAction dropAction = actionNone;
				if (IsValidDropTarget(mousePos, targetPocket, dropAction)) {
					TrySwapPocket(sourcePocket, targetPocket);
					dragAndDropItem->getComponent<InventoryComponent>().pocketNumber = targetPocket;
				}
				dragAndDropItem->getComponent<InventoryComponent>().isInDragAndDrop = false;
				if (dropAction == actionDelete) {
					dragAndDropItem->destroy();
				}
				dragAndDropItem = NULL;
			}
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (!leftMouseButtonDown && m_event.button.button == SDL_BUTTON_LEFT) {
			leftMouseButtonDown = true;

			for (auto &i : invItems) {
				if (i->hasComponent<SpriteComponent>()) {
					auto rect = i->getComponent<SpriteComponent>().getDestRect();
					if (SDL_PointInRect(&mousePos, rect) && i->hasComponent<InventoryComponent>()) {
						dragAndDropItem = i;

						clickOffset.x = mousePos.x - rect->x;
						clickOffset.y = mousePos.y - rect->y;

						i->getComponent<InventoryComponent>().isInDragAndDrop = true;

						break;
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

// Align sprite with underlying pocket sprite
void AlignWithPocket(Entity *const entity)
{
	if (entity->hasComponent<InventoryComponent>()) {
		auto &entityInvC = entity->getComponent<InventoryComponent>();
		if (!entityInvC.isInDragAndDrop) {
			for (auto &e : guiElements) {
				if (e->hasComponent<InventoryComponent>()) {
					if (e->getComponent<InventoryComponent>().pocketNumber == entityInvC.pocketNumber) {
						auto &sourceTransform = e->getComponent<TransformComponent>();
						auto &targetTransform = entity->getComponent<TransformComponent>();

						targetTransform.position = sourceTransform.position;
						targetTransform.position.x += (sourceTransform.width - targetTransform.width / 2);
						targetTransform.position.y += (sourceTransform.height - targetTransform.height / 2);
						targetTransform.scale = sourceTransform.scale - 1;
						break;
					}
				}
			}
		}
	}
}

int getNextEmptyPocket(std::set<int> &pockets)
{
	int maxPockets = xPockets * yPockets;
	int i = 0;
	while (pockets.find(i) != pockets.end()) {
		i++;
	}
	return i;
}

void validatePockets()
{
	std::vector<InventoryComponent *> components;
	for (auto &i : invItems) {
		if (i->hasComponent<InventoryComponent>()) {
			components.emplace_back(&i->getComponent<InventoryComponent>());
		}
	}
	std::set<int> pockets;
	for (auto c : components) {
		// no pocket assigned yet
		if (c->pocketNumber < 0) {
			c->pocketNumber = getNextEmptyPocket(pockets);
		}
		// double assignment
		else if (pockets.find(c->pocketNumber) != pockets.end()) {
			c->pocketNumber = getNextEmptyPocket(pockets);
		}
		pockets.emplace(c->pocketNumber);
	}
}

void RpgInventoryState::Update(RpgGame *rpgGame)
{
	manager.refresh(m_state);
	manager.update(m_state);

	validatePockets();

	for (auto &i : invItems) {
		AlignWithPocket(i);
	}
}

void RpgInventoryState::Render(RpgGame *rpgGame)
{
	SDL_RenderClear(rpgGame->renderer);

	TextureManager::Draw(background, NULL, NULL, SDL_FLIP_NONE, SDL_ALPHA_OPAQUE);

	for (auto &g : guiElements) {
		g->draw(SDL_ALPHA_OPAQUE, m_state);
	}

	for (auto &i : invItems) {
		i->draw(SDL_ALPHA_OPAQUE, m_state);
	}

	if (drawInfo) {
		labelItemName->Draw();
		labelItemInfo->Draw();
	}

	SDL_RenderPresent(rpgGame->renderer);
}
