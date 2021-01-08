
#include "RpgMenuState.hpp"
#include "RpgPlayState.hpp"
#include "rpg/AssetManager.hpp"
#include "rpg/sound/RpgSoundManager.hpp"

SDL_Event RpgMenuState::m_event;

extern Manager manager;

RpgMenuState::RpgMenuState()
{
	std::string playButtonSpritePath = "../rpg/assets/menu/Play.png";
	std::string exitButtonSpritePath = "../rpg/assets/menu/Exit.png";
	std::string backgroundPath = "../rpg/assets/menu/menu_background.png";
	std::string logoPath = "../rpg/assets/menu/logo.png";

	m_menuBackgroundPtr = std::make_shared<RpgMenuItem>(0, 0, RpgGame::SCREEN_HEIGHT, RpgGame::SCREEN_WIDTH,
	                                                    backgroundPath, ITEM_TYPE::BACKGROUND);
	m_logoPtr = std::make_shared<RpgMenuItem>(0, 0, 400, 500, logoPath, ITEM_TYPE::LOGO);

	RpgGame::assets->AddFont("Ancient", "../rpg/assets/font/ancient.ttf", 45);

	auto newGameLabel = std::make_shared<RpgLabel>(LabelType::NEWGAME, "New game", "Ancient", m_colors[0]);
	auto loadGameLabel = std::make_shared<RpgLabel>(LabelType::LOADGAME, "Load game", "Ancient", m_colors[0]);
	auto exitLabel = std::make_shared<RpgLabel>(LabelType::EXIT, "Exit", "Ancient", m_colors[0]);
	m_Labels.push_back(newGameLabel);
	m_Labels.push_back(loadGameLabel);
	m_Labels.push_back(exitLabel);

	m_buttonFucntions.insert({newGameLabel, [](RpgGame *rpgGame) { rpgGame->changeState(RpgPlayState::Instance()); }});
	m_buttonFucntions.insert({loadGameLabel, [](RpgGame *rpgGame) { std::cout << "Load game item clicked! \n"; }});
	m_buttonFucntions.insert({exitLabel, [](RpgGame *rpgGame) { rpgGame->quitGame(); }});

	RpgSoundManager::playMusic("MENU");
}

RpgMenuState::~RpgMenuState() {}

void RpgMenuState::buttonPressed(LabelItem item, RpgGame *rpgGame)
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

		for (int i = 0; i < m_Labels.size(); i++) {
			SDL_Rect menuItemDims;
			m_Labels[i]->getLabelDims(menuItemDims);

			bool insideButton = true;

			if (mousePosX < menuItemDims.x || mousePosX > (menuItemDims.x + menuItemDims.w)
			    || mousePosY < menuItemDims.y || mousePosY > (menuItemDims.y + menuItemDims.h)) {
				insideButton = false;
			}

			if (!insideButton) {
				m_Labels[i]->setLabelColor(m_colors[0]);
			} else {
				switch (m_event.type) {
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONUP:
					m_Labels[i]->setLabelColor(m_colors[1]);
					break;
				case SDL_MOUSEBUTTONDOWN:
					m_Labels[i]->setLabelColor(m_colors[1]);
					buttonPressed(m_Labels[i], rpgGame);
					break;
				default:
					break;
				}
			}
		}

	} break;
	default:
		break;
	}
}

auto &menuProjectiles(manager.getGroup(RpgPlayState::groupProjectiles));

void RpgMenuState::Update(RpgGame *rpgGame)
{
	manager.refresh();
	manager.update();
}

void RpgMenuState::Render(RpgGame *rpgGame)
{
	SDL_RenderClear(rpgGame->renderer);

	m_menuBackgroundPtr->Draw();

	for (auto &p : menuProjectiles) {
		p->draw(SDL_ALPHA_OPAQUE);
	}
	for (auto &l : m_Labels) {
		l->Draw();
	}

	SDL_RenderPresent(rpgGame->renderer);
}
