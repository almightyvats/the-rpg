
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

SDL_Color orange = {188, 21, 21};
SDL_Color white = {63, 63, 63};
bool drawInfo = false;
LabelItem labelItemName;
LabelItem labelItemInfo;
SDL_Rect itemInfoBgSource;
SDL_Rect itemInfoBgTarget;

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

void createCombatantItemSlots(int combatantNumber)
{
	auto spriteInfo = guiHelper->getSpriteInfo("glassPanel_cornerTL.png");
	auto coords = getPocketCoords(9);
	int xDistance = 100;
	int yDistance = 100;
	int baseX = coords.x + (resolution + border) * (scale + 1);
	int baseY = 50 + (combatantNumber - 1) * (2 * yDistance + 32);

	for (int itemSlot = 0; itemSlot < 4; itemSlot++) {
		int x = baseX + (itemSlot % 2) * xDistance;
		int y = baseY + floor(itemSlot / 2) * yDistance;

		auto &item(manager.addEntity(m_state));
		item.addComponent<TransformComponent>(x, y, resolution, resolution, scale + 1);
		item.addComponent<InventoryComponent>((100 * combatantNumber) + itemSlot);
		item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
		                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
		item.addGroup(RpgPlayState::groupGui);
	}
}

RpgInventoryState::RpgInventoryState()
{
	RpgGame::assets->AddFont("Ancient.20", "../rpg/assets/font/ancient.ttf", 20);
	RpgGame::assets->AddTexture("gui", "../rpg/assets/gui/uipackSpace_sheet.png");

	auto spriteInfo = guiHelper->getSpriteInfo("metalPanel.png");
	itemInfoBgSource.x = spriteInfo.x;
	itemInfoBgSource.y = spriteInfo.y;
	itemInfoBgSource.w = spriteInfo.w;
	itemInfoBgSource.h = spriteInfo.h;

	// Inventory slots
	createInventorySlots();
	// Delete item
	createDeleteItemSlot();
	// Combatant items
	createCombatantItemSlots(1);
	createCombatantItemSlots(2);
	createCombatantItemSlots(3);

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

	labelItemName = std::make_shared<RpgLabel>(50, 10, "ItemName", "Ancient.20", orange);
	labelItemInfo = std::make_shared<RpgLabel>(50, 10, "ItemInfo", "Ancient.20", white);
}

RpgInventoryState::~RpgInventoryState() {}

auto &guiElements(manager.getGroup(RpgPlayState::groupGui));
auto &invItems(manager.getGroup(RpgPlayState::groupItems));
auto &equipItemsC1(manager.getGroup(RpgPlayState::groupEquipmentCombatant1));
auto &equipItemsC2(manager.getGroup(RpgPlayState::groupEquipmentCombatant2));
auto &equipItemsC3(manager.getGroup(RpgPlayState::groupEquipmentCombatant3));

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

bool tryShowHint(SDL_Point &mousePos, std::vector<Entity *> &items)
{
	for (auto &i : items) {
		if (i->hasComponent<SpriteComponent>()) {
			auto rect = i->getComponent<SpriteComponent>().getDestRect();
			if (SDL_PointInRect(&mousePos, rect) && i->hasComponent<InventoryComponent>()) {
				labelItemName->setLabelPos(Vector2D(rect->x + 2 * resolution, rect->y));
				labelItemName->setLabelText("Ancient.20", i->getComponent<InventoryComponent>().equip.name());
				SDL_Rect dimsName;
				labelItemName->getLabelDims(dimsName);

				labelItemInfo->setLabelPos(
				    Vector2D(rect->x + 2 * resolution, dimsName.y + dimsName.h + border * scale));
				labelItemInfo->setLabelText("Ancient.20", i->getComponent<InventoryComponent>().equip.item_info());
				SDL_Rect dimsInfo;
				labelItemInfo->getLabelDims(dimsInfo);

				itemInfoBgTarget.x = (rect->x + 2 * resolution) - 20;
				itemInfoBgTarget.y = rect->y - 20;
				itemInfoBgTarget.w = 3 * resolution * scale;
				itemInfoBgTarget.h = dimsName.h + dimsInfo.h + (border * scale) + 40;

				return true;
			}
		}
	}
	return false;
}

bool tryShowHint(SDL_Point &mousePos)
{
	auto result = tryShowHint(mousePos, invItems);
	result |= tryShowHint(mousePos, equipItemsC1);
	result |= tryShowHint(mousePos, equipItemsC2);
	result |= tryShowHint(mousePos, equipItemsC3);

	return result;
}

Entity *dragAndDropItem = NULL;
bool buttonClicked = false;
bool leftMouseButtonDown = false;

SDL_Point clickOffset;

SDL_Point mousePos;

void tryAssignDragDropItem(std::vector<Entity *> &items)
{
	if (dragAndDropItem == NULL) {
		for (auto &i : items) {
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
}
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

			tryAssignDragDropItem(invItems);
			tryAssignDragDropItem(equipItemsC1);
			tryAssignDragDropItem(equipItemsC2);
			tryAssignDragDropItem(equipItemsC3);
		}
		break;
	default:
		break;
	}
}

// Align sprite with underlying pocket sprite
void alignWithPocket(Entity *const entity)
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

void alignWithPocket(std::vector<Entity *> &items)
{
	for (auto &i : items) {
		alignWithPocket(i);
	}
}

bool inRange(int low, int high, int x)
{
	return ((x - high) * (x - low) <= 0);
}

void assignInventoryGroup(Entity *entity, Group currentGroup, int pocketNumber)
{
	// Inventory - not equipped
	if (inRange(0, 99, pocketNumber) && currentGroup != RpgPlayState::groupItems) {
		entity->addGroup(RpgPlayState::groupItems);
		entity->delGroup(RpgPlayState::groupEquipmentCombatant1);
		entity->delGroup(RpgPlayState::groupEquipmentCombatant2);
		entity->delGroup(RpgPlayState::groupEquipmentCombatant3);
	}
	// 100 - 199 - items combatant 1
	else if (inRange(100, 199, pocketNumber) && currentGroup != RpgPlayState::groupEquipmentCombatant1) {
		entity->delGroup(RpgPlayState::groupItems);
		entity->addGroup(RpgPlayState::groupEquipmentCombatant1);
		entity->delGroup(RpgPlayState::groupEquipmentCombatant2);
		entity->delGroup(RpgPlayState::groupEquipmentCombatant3);
	}
	// 200 - 299 - items combatant 2
	else if (inRange(200, 299, pocketNumber) && currentGroup != RpgPlayState::groupEquipmentCombatant2) {
		entity->delGroup(RpgPlayState::groupItems);
		entity->delGroup(RpgPlayState::groupEquipmentCombatant1);
		entity->addGroup(RpgPlayState::groupEquipmentCombatant2);
		entity->delGroup(RpgPlayState::groupEquipmentCombatant3);
	}
	// 300 - 399 - items combatant 3
	else if (inRange(300, 399, pocketNumber) && currentGroup != RpgPlayState::groupEquipmentCombatant3) {
		entity->delGroup(RpgPlayState::groupItems);
		entity->delGroup(RpgPlayState::groupEquipmentCombatant1);
		entity->delGroup(RpgPlayState::groupEquipmentCombatant2);
		entity->addGroup(RpgPlayState::groupEquipmentCombatant3);
	}
}

void assignInventoryGroup()
{
	for (auto &i : invItems) {
		if (i->hasComponent<InventoryComponent>()) {
			assignInventoryGroup(i, RpgPlayState::groupItems, i->getComponent<InventoryComponent>().pocketNumber);
		}
	}
	for (auto &i : equipItemsC1) {
		if (i->hasComponent<InventoryComponent>()) {
			assignInventoryGroup(i, RpgPlayState::groupEquipmentCombatant1,
			                     i->getComponent<InventoryComponent>().pocketNumber);
		}
	}
	for (auto &i : equipItemsC2) {
		if (i->hasComponent<InventoryComponent>()) {
			assignInventoryGroup(i, RpgPlayState::groupEquipmentCombatant2,
			                     i->getComponent<InventoryComponent>().pocketNumber);
		}
	}
	for (auto &i : equipItemsC3) {
		if (i->hasComponent<InventoryComponent>()) {
			assignInventoryGroup(i, RpgPlayState::groupEquipmentCombatant3,
			                     i->getComponent<InventoryComponent>().pocketNumber);
		}
	}
}

void alignWithPocket()
{
	alignWithPocket(invItems);
	alignWithPocket(equipItemsC2);
	alignWithPocket(equipItemsC1);
	alignWithPocket(equipItemsC3);
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

void retrieveComponents(std::vector<Entity *> &items, std::vector<InventoryComponent *> &components)
{
	for (auto &i : items) {
		if (i->hasComponent<InventoryComponent>()) {
			components.emplace_back(&i->getComponent<InventoryComponent>());
		}
	}
}
void validatePockets()
{
	std::vector<InventoryComponent *> components;
	retrieveComponents(invItems, components);
	retrieveComponents(equipItemsC1, components);
	retrieveComponents(equipItemsC2, components);
	retrieveComponents(equipItemsC3, components);

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
	assignInventoryGroup();
	alignWithPocket();
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

	for (auto &c1 : equipItemsC1) {
		c1->draw(SDL_ALPHA_OPAQUE, m_state);
	}

	for (auto &c2 : equipItemsC2) {
		c2->draw(SDL_ALPHA_OPAQUE, m_state);
	}

	for (auto &c3 : equipItemsC3) {
		c3->draw(SDL_ALPHA_OPAQUE, m_state);
	}

	if (drawInfo) {

		TextureManager::Draw(RpgGame::assets->GetTexture("gui"), &itemInfoBgSource, &itemInfoBgTarget, SDL_FLIP_NONE);
		labelItemName->Draw();
		labelItemInfo->Draw();
	}

	SDL_RenderPresent(rpgGame->renderer);
}
