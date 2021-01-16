
#include "RpgInventoryState.hpp"
#include "RpgStates.hpp"
#include "rpg/AssetManager.hpp"
#include "rpg/RpgSoundManager.hpp"

SDL_Event RpgInventoryState::m_event;

extern Manager manager;
const State m_state = stateInventory;

RpgInventoryState::RpgInventoryState()
{
	RpgGame::assets->AddTexture("icons", "../rpg/assets/icons/Icon Pack_3.png");

	RpgGame::assets->CreateItem(0, 0, 0, 0, "icons", m_state);

	RpgGame::assets->AddTexture("fireball", "../rpg/assets/fireball_sprite.png");
	RpgGame::assets->CreateProjectile(Vector2D(700, 400), Vector2D(0, 0), 200, 0, "fireball", m_state);
	RpgGame::assets->CreateProjectile(Vector2D(200, 200), Vector2D(0, 0), 200, 0, "fireball", m_state);

	// auto menuItem1 = std::make_shared<RpgMenuItem>(0, 0, 61, 43, volumeButtonSpritePath, ITEM_TYPE::MUTE_BUTTON);
	// m_volumeButton.push_back(menuItem1);
	// auto menuItem2 = std::make_shared<RpgMenuItem>(43, 0, 61, 52, volumeButtonSpritePath, ITEM_TYPE::UNMUTE_BUTTON);
	// m_volumeButton.push_back(menuItem2);

	// m_muteBtnWithStates.insert({m_volumeButton, MUTE_BUTTON_STATE::BUTTON_SPRITE_UNMUTED});

	// m_menuBackgroundPtr = std::make_shared<RpgMenuItem>(0, 0, RpgGame::SCREEN_HEIGHT, RpgGame::SCREEN_WIDTH,
	//                                                     backgroundPath, ITEM_TYPE::BACKGROUND);
	// m_logoPtr = std::make_shared<RpgMenuItem>(0, 0, 400, 500, logoPath, ITEM_TYPE::LOGO);

	// RpgGame::assets->AddFont("Ancient", "../rpg/assets/font/ancient.ttf", 45);

	// auto newGameLabel = std::make_shared<RpgLabel>(LabelType::NEWGAME, "New game", "Ancient", m_colors[0]);
	// auto loadGameLabel = std::make_shared<RpgLabel>(LabelType::LOADGAME, "Load game", "Ancient", m_colors[0]);
	// auto exitLabel = std::make_shared<RpgLabel>(LabelType::EXIT, "Exit", "Ancient", m_colors[0]);
	// m_Labels.push_back(newGameLabel);
	// m_Labels.push_back(loadGameLabel);
	// m_Labels.push_back(exitLabel);

	// m_buttonFucntions.insert({newGameLabel, [](RpgGame *rpgGame) { rpgGame->changeState(RpgPlayState::Instance());
	// }}); m_buttonFucntions.insert({loadGameLabel, [](RpgGame *rpgGame) { std::cout << "Load game item clicked! \n";
	// }}); m_buttonFucntions.insert({exitLabel, [](RpgGame *rpgGame) { rpgGame->quitGame(); }});

	// RpgSoundManager::setMusicVolume(15);
	// int oldVolume = RpgSoundManager::getMusicVolume();
	// auto volumeMinusLabel = std::make_shared<RpgLabel>(50, 10, "-", "Ancient", m_colors[0]);
	// // auto volumeNumberLabel = std::make_shared<RpgLabel>(150, 10, std::to_string(oldVolume), "Ancient",
	// m_colors[0]); auto volumePlusLabel = std::make_shared<RpgLabel>(200, 10, "+", "Ancient", m_colors[0]);

	// m_buttonFucntions.insert({volumeMinusLabel, [](RpgGame *rpgGame) {
	// 	                          if (!RpgSoundManager::isMusicMuted()) {
	// 		                          int oldVolume = RpgSoundManager::getMusicVolume();
	// 		                          int newVolume = (oldVolume - 15 <= 0) ? 0 : oldVolume - 15;
	// 		                          RpgSoundManager::setMusicVolume(newVolume);
	// 	                          }
	//                           }});

	// m_buttonFucntions.insert({volumePlusLabel, [](RpgGame *rpgGame) {
	// 	                          if (!RpgSoundManager::isMusicMuted()) {
	// 		                          int oldVolume = RpgSoundManager::getMusicVolume();
	// 		                          int newVolume =
	// 		                              (oldVolume + 15 >= MIX_MAX_VOLUME) ? MIX_MAX_VOLUME : oldVolume + 15;
	// 		                          RpgSoundManager::setMusicVolume(newVolume);
	// 	                          }
	//                           }});

	// m_Labels.push_back(volumeMinusLabel);
	// // m_Labels.push_back(volumeNumberLabel);
	// m_Labels.push_back(volumePlusLabel);

	// RpgSoundManager::playMusic("MENU");
}

RpgInventoryState::~RpgInventoryState() {}

auto &invItems(manager.getGroup(RpgPlayState::groupItems));
auto &invProjectiles(manager.getGroup(RpgPlayState::groupProjectiles));

void RpgInventoryState::Pause()
{
	std::cout << "pause" << std::endl;
	RpgSoundManager::pauseMusic();
}

void RpgInventoryState::Resume()
{
	std::cout << "resume" << std::endl;
	RpgSoundManager::resumeMusic("PLAY");
	RpgGame::assets->CreateProjectile(Vector2D(200, 200), Vector2D(0, 0), 200, 0, "fireball", m_state);
}

void RpgInventoryState::HandleEvents(RpgGame *rpgGame)
{
	bool buttonClicked = false;

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
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP: {
		// int mousePosX, mousePosY;
		// SDL_GetMouseState(&mousePosX, &mousePosY);

		// for (int i = 0; i < m_Labels.size(); i++) {
		// 	SDL_Rect menuItemDims;
		// 	m_Labels[i]->getLabelDims(menuItemDims);

		// 	bool insideLabel = true;

		// 	if (mousePosX < menuItemDims.x || mousePosX > (menuItemDims.x + menuItemDims.w)
		// 	    || mousePosY < menuItemDims.y || mousePosY > (menuItemDims.y + menuItemDims.h)) {
		// 		insideLabel = false;
		// 	}

		// 	if (!insideLabel) {
		// 		m_Labels[i]->setLabelColor(m_colors[0]);
		// 	} else {
		// 		switch (m_event.type) {
		// 		case SDL_MOUSEMOTION:
		// 		case SDL_MOUSEBUTTONUP:
		// 			m_Labels[i]->setLabelColor(m_colors[1]);
		// 			isLabelClicked = false;
		// 			break;
		// 		case SDL_MOUSEBUTTONDOWN:
		// 			m_Labels[i]->setLabelColor(m_colors[1]);
		// 			if (!isLabelClicked) {
		// 				labelPressed(m_Labels[i], rpgGame);
		// 				isLabelClicked = true;
		// 			}
		// 			break;
		// 		default:
		// 			break;
		// 		}
		// 	}
		// }

		// SDL_Rect menuItemDims;
		// m_volumeButton[0]->getMenuItemDims(menuItemDims);

		// bool insideMuteButton = true;

		// if (mousePosX < menuItemDims.x || mousePosX > (menuItemDims.x + menuItemDims.w) || mousePosY < menuItemDims.y
		//     || mousePosY > (menuItemDims.y + menuItemDims.h)) {
		// 	insideMuteButton = false;
		// }

		// if (!insideMuteButton) {

		// } else {
		// 	switch (m_event.type) {
		// 	case SDL_MOUSEBUTTONUP:
		// 		isMuteBtnClicked = false;
		// 		break;
		// 	case SDL_MOUSEBUTTONDOWN:
		// 		if (!isMuteBtnClicked) {
		// 			m_muteBtnWithStates[m_volumeButton] =
		// 			    m_muteBtnWithStates[m_volumeButton] == MUTE_BUTTON_STATE::BUTTON_SPRITE_UNMUTED
		// 			        ? MUTE_BUTTON_STATE::BUTTON_SPRITE_MUTED
		// 			        : MUTE_BUTTON_STATE::BUTTON_SPRITE_UNMUTED;
		// 			muteBtnPressed();
		// 			isMuteBtnClicked = true;
		// 		}
		// 		break;
		// 	default:
		// 		break;
		// 	}
		// }

	} break;
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

	// m_menuBackgroundPtr->Draw();

	// for (auto &l : m_Labels) {
	// 	l->Draw();
	// }

	// for (const auto &btn : m_muteBtnWithStates) {
	// 	btn.first[btn.second]->Draw();
	// }

	for (auto &i : invItems) {
		i->draw(SDL_ALPHA_OPAQUE, m_state);
	}

	for (auto &p : invProjectiles) {
		p->draw(SDL_ALPHA_OPAQUE, m_state);
	}

	SDL_RenderPresent(rpgGame->renderer);
}
