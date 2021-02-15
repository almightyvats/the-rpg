
#include "../combat/CombatTest.hpp"
#include "../combat/Combatant.hpp"
#include "RpgCombatState.hpp"
#include "RpgPlayerConvoState.hpp"
#include "RpgStates.hpp"
#include "rpg/AssetManager.hpp"
#include "rpg/RpgSoundManager.hpp"
#include "rpg/SaveGame.hpp"

SDL_Event RpgMenuState::m_event;

extern Manager manager;
extern SaveGame saveGame;
const State m_state = stateMenu;

RpgMenuState::RpgMenuState()
{
	std::string volumeButtonSpritePath = "../rpg/assets/menu/volume.png";
	std::string backgroundPath = "../rpg/assets/menu/menu_background.png";
	std::string logoPath = "../rpg/assets/menu/logo.png";

	auto menuItem1 = std::make_shared<RpgMenuItem>(0, 0, 61, 43, volumeButtonSpritePath, ITEM_TYPE::MUTE_BUTTON);
	m_volumeButton.push_back(menuItem1);
	auto menuItem2 = std::make_shared<RpgMenuItem>(43, 0, 61, 52, volumeButtonSpritePath, ITEM_TYPE::UNMUTE_BUTTON);
	m_volumeButton.push_back(menuItem2);

	m_muteBtnWithStates.insert({m_volumeButton, MUTE_BUTTON_STATE::BUTTON_SPRITE_UNMUTED});

	m_menuBackgroundPtr = std::make_shared<RpgMenuItem>(0, 0, RpgGame::SCREEN_HEIGHT, RpgGame::SCREEN_WIDTH,
	                                                    backgroundPath, ITEM_TYPE::BACKGROUND);
	m_logoPtr = std::make_shared<RpgMenuItem>(0, 0, 400, 500, logoPath, ITEM_TYPE::LOGO);

	RpgGame::assets->AddFont("Ancient", "../rpg/assets/font/ancient.ttf", 45);
	RpgGame::assets->AddFont("Conversation", "../rpg/assets/font/conversation.ttf", 30);

	auto newGameLabel = std::make_shared<RpgLabel>(LabelType::NEWGAME, "New game", "Ancient", m_colors[0]);
	auto loadGameLabel = std::make_shared<RpgLabel>(LabelType::LOADGAME, "Load game", "Ancient", m_colors[0]);
	auto exitLabel = std::make_shared<RpgLabel>(LabelType::EXIT, "Exit", "Ancient", m_colors[0]);
	m_Labels.push_back(newGameLabel);
	m_Labels.push_back(loadGameLabel);
	m_Labels.push_back(exitLabel);

	m_buttonFucntions.insert({newGameLabel, [](RpgGame *rpgGame) {
		                          saveGame = SaveGame();
		                          saveGame.NewGame();
		                          rpgGame->changeState(RpgPlayState::Instance());
	                          }});

	/*m_buttonFucntions.insert({newGameLabel, [](RpgGame *rpgGame) {
	    InitGlobalTestPCs();
	    rpgGame->changeState(RpgCombatState::Instance(GetTestCombatants(), CombatArena::grass)); }});*/

	m_buttonFucntions.insert(
	    {loadGameLabel, [](RpgGame *rpgGame) { rpgGame->pushState(RpgLoadGameState::Instance()); }});
	m_buttonFucntions.insert({exitLabel, [](RpgGame *rpgGame) { rpgGame->quitGame(); }});

	RpgSoundManager::setMusicVolume(15);
	int oldVolume = RpgSoundManager::getMusicVolume();
	auto volumeMinusLabel = std::make_shared<RpgLabel>(50, 10, "-", "Ancient", m_colors[0]);
	// auto volumeNumberLabel = std::make_shared<RpgLabel>(150, 10, std::to_string(oldVolume), "Ancient", m_colors[0]);
	auto volumePlusLabel = std::make_shared<RpgLabel>(200, 10, "+", "Ancient", m_colors[0]);

	m_buttonFucntions.insert({volumeMinusLabel, [](RpgGame *rpgGame) {
		                          if (!RpgSoundManager::isMusicMuted()) {
			                          int oldVolume = RpgSoundManager::getMusicVolume();
			                          int newVolume = (oldVolume - 15 <= 0) ? 0 : oldVolume - 15;
			                          RpgSoundManager::setMusicVolume(newVolume);
		                          }
	                          }});

	m_buttonFucntions.insert({volumePlusLabel, [](RpgGame *rpgGame) {
		                          if (!RpgSoundManager::isMusicMuted()) {
			                          int oldVolume = RpgSoundManager::getMusicVolume();
			                          int newVolume =
			                              (oldVolume + 15 >= MIX_MAX_VOLUME) ? MIX_MAX_VOLUME : oldVolume + 15;
			                          RpgSoundManager::setMusicVolume(newVolume);
		                          }
	                          }});

	m_Labels.push_back(volumeMinusLabel);
	// m_Labels.push_back(volumeNumberLabel);
	m_Labels.push_back(volumePlusLabel);

	RpgSoundManager::playMusic("MENU");
}

RpgMenuState::~RpgMenuState() {}

void RpgMenuState::labelPressed(LabelItem item, RpgGame *rpgGame)
{
	for (auto button : m_buttonFucntions) {
		if (button.first == item) {
			button.second(rpgGame);
		}
	}
}

void RpgMenuState::muteBtnPressed()
{
	RpgSoundManager::toggleMuteVolume();
}

void RpgMenuState::Pause()
{
	RpgSoundManager::pauseMusic();
}

void RpgMenuState::Resume()
{
	RpgSoundManager::resumeMusic("MENU");
}

bool isLabelClicked = false;
bool isMuteBtnClicked = false;

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
			break;
		case SDLK_d:

			// const SDL_MessageBoxButtonData buttons[] = {
			//     {/* .flags, .buttonid, .text */ 0, 0, "no"},
			//     {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes"},
			//     {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel"},
			// };
			// const SDL_MessageBoxColorScheme colorScheme = {{/* .colors (.r, .g, .b) */
			//                                                 /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
			//                                                 {255, 0, 0},
			//                                                 /* [SDL_MESSAGEBOX_COLOR_TEXT] */
			//                                                 {0, 255, 0},
			//                                                 /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
			//                                                 {255, 255, 0},
			//                                                 /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
			//                                                 {0, 0, 255},
			//                                                 /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
			//                                                 {255, 0, 255}}};
			// const SDL_MessageBoxData messageboxdata = {
			//     SDL_MESSAGEBOX_INFORMATION, /* .flags */
			//     NULL,                       /* .window */
			//     "example message box",      /* .title */
			//     "select a button",          /* .message */
			//     SDL_arraysize(buttons),     /* .numbuttons */
			//     buttons,                    /* .buttons */
			//     &colorScheme                /* .colorScheme */
			// };
			// int buttonid;
			// if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
			// 	SDL_Log("error displaying message box");
			// 	break;
			// }
			// if (buttonid == -1) {
			// 	SDL_Log("no selection");
			// } else {
			// 	SDL_Log("selection was %s", buttons[buttonid].text);
			// }
			break;
		}
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP: {
		int mousePosX, mousePosY;
		SDL_GetMouseState(&mousePosX, &mousePosY);

		for (int i = 0; i < m_Labels.size(); i++) {
			SDL_Rect menuItemDims;
			m_Labels[i]->getLabelDims(menuItemDims);

			bool insideLabel = true;

			if (mousePosX < menuItemDims.x || mousePosX > (menuItemDims.x + menuItemDims.w)
			    || mousePosY < menuItemDims.y || mousePosY > (menuItemDims.y + menuItemDims.h)) {
				insideLabel = false;
			}

			if (!insideLabel) {
				m_Labels[i]->setLabelColor(m_colors[0]);
			} else {
				switch (m_event.type) {
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONUP:
					m_Labels[i]->setLabelColor(m_colors[1]);
					isLabelClicked = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					m_Labels[i]->setLabelColor(m_colors[1]);
					if (!isLabelClicked) {
						labelPressed(m_Labels[i], rpgGame);
						isLabelClicked = true;
					}
					break;
				default:
					break;
				}
			}
		}

		SDL_Rect menuItemDims;
		m_volumeButton[0]->getMenuItemDims(menuItemDims);

		bool insideMuteButton = true;

		if (mousePosX < menuItemDims.x || mousePosX > (menuItemDims.x + menuItemDims.w) || mousePosY < menuItemDims.y
		    || mousePosY > (menuItemDims.y + menuItemDims.h)) {
			insideMuteButton = false;
		}

		if (!insideMuteButton) {

		} else {
			switch (m_event.type) {
			case SDL_MOUSEBUTTONUP:
				isMuteBtnClicked = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (!isMuteBtnClicked) {
					m_muteBtnWithStates[m_volumeButton] =
					    m_muteBtnWithStates[m_volumeButton] == MUTE_BUTTON_STATE::BUTTON_SPRITE_UNMUTED
					        ? MUTE_BUTTON_STATE::BUTTON_SPRITE_MUTED
					        : MUTE_BUTTON_STATE::BUTTON_SPRITE_UNMUTED;
					muteBtnPressed();
					isMuteBtnClicked = true;
				}
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
	manager.refresh(m_state);
	manager.update(m_state);
}

void RpgMenuState::Render(RpgGame *rpgGame)
{
	SDL_RenderClear(rpgGame->renderer);

	m_menuBackgroundPtr->Draw();

	for (auto &l : m_Labels) {
		l->Draw();
	}

	for (const auto &btn : m_muteBtnWithStates) {
		btn.first[btn.second]->Draw();
	}

	SDL_RenderPresent(rpgGame->renderer);
}
