
#include "RpgInventoryState.hpp"
#include "RpgStates.hpp"
#include "rpg/AssetManager.hpp"
#include "rpg/RpgSoundManager.hpp"
#include "rpg/ecs/Components.hpp"
#include "rpg/inventory/GuiHelper.hpp"

SDL_Event RpgInventoryState::m_event;
GuiHelper *guiHelper = new GuiHelper("../rpg/assets/gui/uipackSpace_sheet.json");

extern Manager manager;
const State m_state = stateInventory;

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

RpgInventoryState::RpgInventoryState()
{
	RpgGame::assets->AddTexture("gui", "../rpg/assets/gui/uipackSpace_sheet.png");
	auto spriteInfo = guiHelper->getSpriteInfo("glassPanel.png");

	for (int i = 0; i < 56; i++) {
		auto coords = getPocketCoords(i);
		auto &item(manager.addEntity(m_state));
		item.addComponent<TransformComponent>(coords.x, coords.y, resolution, resolution, scale);
		item.addComponent<InventoryComponent>(i, false);
		item.addComponent<SpriteComponent>("gui", spriteInfo.x, spriteInfo.y,
		                                   SpriteSheet(1, spriteInfo.w, spriteInfo.h, 0, 0));
		item.addGroup(RpgPlayState::groupGui);
	}

	RpgGame::assets->AddTexture("icons", "../rpg/assets/icons/Icon Pack_3.png");
	RpgGame::assets->CreateInventoryItem(0, 0, 0, "icons", m_state);
	RpgGame::assets->CreateInventoryItem(0, 4, 1, "icons", m_state);
	RpgGame::assets->CreateInventoryItem(5, 0, 2, "icons", m_state);
	RpgGame::assets->CreateInventoryItem(7, 7, 3, "icons", m_state);
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

bool IsValidDropTarget(SDL_Point mousePos, int &target)
{
	for (auto &i : guiElements) {
		if (i->hasComponent<SpriteComponent>() && i->hasComponent<InventoryComponent>()) {
			auto rect = i->getComponent<SpriteComponent>().getDestRect();
			if (SDL_PointInRect(&mousePos, rect)) {
				target = i->getComponent<InventoryComponent>().pocketNumber;
				return true;
			}
		}
	}
	return false;
}

 //if another Item is at target position - swap positions
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
				if (IsValidDropTarget(mousePos, targetPocket)) {
					TrySwapPocket(sourcePocket, targetPocket);
					dragAndDropItem->getComponent<InventoryComponent>().pocketNumber = targetPocket;
				}

				

				// TODO: wenn ungültiges target - position reset
				// wenn anderes item - position swap
				dragAndDropItem->getComponent<InventoryComponent>().isInDragAndDrop = false;
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

void RpgInventoryState::Update(RpgGame *rpgGame)
{
	manager.refresh(m_state);
	manager.update(m_state);
}

void RpgInventoryState::Render(RpgGame *rpgGame)
{
	SDL_RenderClear(rpgGame->renderer);

	for (auto &g : guiElements) {
		g->draw(SDL_ALPHA_OPAQUE, m_state);
	}

	for (auto &i : invItems) {
		i->draw(SDL_ALPHA_OPAQUE, m_state);
	}

	SDL_RenderPresent(rpgGame->renderer);
}
