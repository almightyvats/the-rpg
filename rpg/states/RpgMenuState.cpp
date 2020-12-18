
#include "RpgMenuState.hpp"
#include "RpgPlayState.hpp"
#include "rpg/AssetManager.hpp"
#include <iostream>

SDL_Event RpgMenuState::m_event;

RpgMenuState::RpgMenuState()
{
	std::string menuButtonSpritePath = "../rpg/assets/menu/button.png";

	for (int i = 0; i < BUTTON_STATE::BUTTON_SPRITE_TOTAL; i++) {
		auto menuItem = std::make_shared<RpgMenuItem>(0, i * BUTTON_PROPS::BUTTON_HEIGHT, BUTTON_PROPS::BUTTON_HEIGHT,
		                                              BUTTON_PROPS::BUTTON_WIDTH, menuButtonSpritePath);
		// m_menuItems.insert({i, menuItem});
		m_menuItems.push_back(menuItem);
	}

	m_currentSprite = BUTTON_STATE::BUTTON_SPRITE_MOUSE_OUT;
}

RpgMenuState::~RpgMenuState() {}

void RpgMenuState::Pause() {}
void RpgMenuState::Resume() {}

void RpgMenuState::HandleEvents(RpgGame *rpgGame)
{
	SDL_PollEvent(&m_event);
	switch (m_event.type) {
	case SDL_QUIT:
		rpgGame->quitGame();
		break;
	case SDL_KEYDOWN:
		switch (m_event.key.keysym.sym) {
		case SDLK_SPACE:
			rpgGame->changeState(RpgPlayState::Instance());
		}
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP: {
		int mousePosX, mousePosY;
		SDL_GetMouseState(&mousePosX, &mousePosY);

		SDL_Point menuItemPosition = m_menuItems[m_currentSprite]->menuItemPosition();

		bool inside = true;

		if (mousePosX < menuItemPosition.x || mousePosX > (menuItemPosition.x + BUTTON_PROPS::BUTTON_WIDTH)
		    || mousePosY < menuItemPosition.y || mousePosY > (menuItemPosition.y + BUTTON_PROPS::BUTTON_HEIGHT)) {
			inside = false;
		}

		if (!inside) {
			m_currentSprite = BUTTON_STATE::BUTTON_SPRITE_MOUSE_OUT;
		} else {
			switch (m_event.type) {
			case SDL_MOUSEMOTION:
				m_currentSprite = BUTTON_STATE::BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			case SDL_MOUSEBUTTONUP:
				m_currentSprite = BUTTON_STATE::BUTTON_SPRITE_MOUSE_UP;
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_currentSprite = BUTTON_STATE::BUTTON_SPRITE_MOUSE_DOWN;
				break;
			default:
				break;
			}
		}

	} break;
	default:
		break;
	}
}

void RpgMenuState::Update(RpgGame *rpgGame)
{
	m_menuItems[m_currentSprite]->Update();
	// for (auto &menuItem : m_menuItems) {
	// 	menuItem->Update();
	// }
}

void RpgMenuState::Render(RpgGame *rpgGame)
{
	SDL_RenderClear(rpgGame->renderer);

	m_menuItems[m_currentSprite]->Draw();
	// for (auto &menuItem : m_menuItems) {
	// 	menuItem->Draw();
	// }

	SDL_RenderPresent(rpgGame->renderer);
}
