
#include "RpgMenuState.hpp"
#include "RpgPlayState.hpp"
#include "rpg/AssetManager.hpp"
#include "rpg/RpgSoundManager.hpp"

SDL_Event RpgMenuState::m_event;

RpgMenuState::RpgMenuState()
{
	std::string playButtonSpritePath = "../rpg/assets/menu/Play.png";
	std::string exitButtonSpritePath = "../rpg/assets/menu/Exit.png";
	std::string backgroundPath = "../rpg/assets/menu/menu_background.png";
	std::string logoPath = "../rpg/assets/menu/logo.png";

	for (int i = 0; i < BUTTON_STATE::BUTTON_SPRITE_TOTAL; i++) {
		auto menuItem =
		    std::make_shared<RpgMenuItem>(0, i * (BUTTON_PROPS::BUTTON_HEIGHT + 120), BUTTON_PROPS::BUTTON_HEIGHT,
		                                  BUTTON_PROPS::BUTTON_WIDTH, playButtonSpritePath, ITEM_TYPE::PLAY_BUTTON);
		m_playButton.push_back(menuItem);
	}

	for (int i = 0; i < BUTTON_STATE::BUTTON_SPRITE_TOTAL; i++) {
		auto menuItem =
		    std::make_shared<RpgMenuItem>(0, i * (BUTTON_PROPS::BUTTON_HEIGHT + 150), BUTTON_PROPS::BUTTON_HEIGHT,
		                                  BUTTON_PROPS::BUTTON_WIDTH, exitButtonSpritePath, ITEM_TYPE::EXIT_BUTTON);
		m_exitButton.push_back(menuItem);
	}

	m_buttonFucntions.insert({m_playButton, [](RpgGame *rpgGame) { rpgGame->changeState(RpgPlayState::Instance()); }});
	m_buttonFucntions.insert({m_exitButton, [](RpgGame *rpgGame) { rpgGame->quitGame(); }});

	m_menuBackgroundPtr = std::make_shared<RpgMenuItem>(0, 0, RpgGame::SCREEN_HEIGHT, RpgGame::SCREEN_WIDTH,
	                                                    backgroundPath, ITEM_TYPE::BACKGROUND);
	m_logoPtr = std::make_shared<RpgMenuItem>(0, 0, 400, 500, logoPath, ITEM_TYPE::LOGO);

	m_buttonsWithState.insert({m_playButton, BUTTON_STATE::BUTTON_SPRITE_MOUSE_OUT});
	m_buttonsWithState.insert({m_exitButton, BUTTON_STATE::BUTTON_SPRITE_MOUSE_OUT});

	RpgSoundManager::playMusic("MENU");
}

RpgMenuState::~RpgMenuState() {}

void RpgMenuState::buttonPressed(MenuItem item, RpgGame *rpgGame)
{
	for (auto button : m_buttonFucntions) {
		if (button.first == item) {
			button.second(rpgGame);
		}
	}
}

void RpgMenuState::Pause()
{
	RpgSoundManager::pauseMusic();
}

void RpgMenuState::Resume()
{
	RpgSoundManager::resumeMusic("MENU");
}

void RpgMenuState::HandleEvents(RpgGame *rpgGame)
{
	bool buttonClicked = false;

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

		SDL_Point menuItemPosition = m_playButton[m_currentSprite]->menuItemPosition();

		SDL_Point exitButtonPosition = m_exitButton[m_currentSprite]->menuItemPosition();

		bool insidePlayButton = true;
		bool insideExitButton = true;

		if (mousePosX < menuItemPosition.x || mousePosX > (menuItemPosition.x + BUTTON_PROPS::BUTTON_WIDTH)
		    || mousePosY < menuItemPosition.y || mousePosY > (menuItemPosition.y + BUTTON_PROPS::BUTTON_HEIGHT)) {
			insidePlayButton = false;
		}

		if (mousePosX < exitButtonPosition.x || mousePosX > (exitButtonPosition.x + BUTTON_PROPS::BUTTON_WIDTH)
		    || mousePosY < exitButtonPosition.y || mousePosY > (exitButtonPosition.y + BUTTON_PROPS::BUTTON_HEIGHT)) {
			insideExitButton = false;
		}

		if (!insidePlayButton) {
			m_buttonsWithState[m_playButton] = BUTTON_STATE::BUTTON_SPRITE_MOUSE_OUT;
		} else {
			switch (m_event.type) {
			case SDL_MOUSEMOTION:
				m_buttonsWithState[m_playButton] = BUTTON_STATE::BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			case SDL_MOUSEBUTTONUP:
				m_buttonsWithState[m_playButton] = BUTTON_STATE::BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_buttonsWithState[m_playButton] = BUTTON_STATE::BUTTON_SPRITE_MOUSE_DOWN;
				buttonPressed(m_playButton, rpgGame);
				break;
			default:
				break;
			}
		}

		if (!insideExitButton) {
			m_buttonsWithState[m_exitButton] = BUTTON_STATE::BUTTON_SPRITE_MOUSE_OUT;
		} else {
			switch (m_event.type) {
			case SDL_MOUSEMOTION:
				m_buttonsWithState[m_exitButton] = BUTTON_STATE::BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			case SDL_MOUSEBUTTONUP:
				m_buttonsWithState[m_exitButton] = BUTTON_STATE::BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_buttonsWithState[m_exitButton] = BUTTON_STATE::BUTTON_SPRITE_MOUSE_DOWN;
				buttonPressed(m_exitButton, rpgGame);
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
	m_playButton[m_currentSprite]->Update();
	m_exitButton[m_currentSprite]->Update();
}

void RpgMenuState::Render(RpgGame *rpgGame)
{
	SDL_RenderClear(rpgGame->renderer);

	m_menuBackgroundPtr->Draw();
	// m_logoPtr->Draw();
	for (const auto &button : m_buttonsWithState) {
		auto item = button.first;
		auto state = button.second;
		item[state]->Draw();
	}

	SDL_RenderPresent(rpgGame->renderer);
}
