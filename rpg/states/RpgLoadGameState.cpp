#include "RpgLoadGameState.hpp"
#include "../SaveGame.hpp"
#include <filesystem>

namespace fs = std::filesystem;

RpgLoadGameState::RpgLoadGameState()
{
	m_dialogueBox1.x = 25;
	m_dialogueBox1.y = RpgGame::SCREEN_HEIGHT - 200;
	m_dialogueBox1.w = RpgGame::SCREEN_WIDTH - 50;
	m_dialogueBox1.h = 150;

	m_dialogueBox2.x = 30;
	m_dialogueBox2.y = RpgGame::SCREEN_HEIGHT - 190;
	m_dialogueBox2.w = RpgGame::SCREEN_WIDTH - 60;
	m_dialogueBox2.h = 130;

	RpgGame::assets->AddFont("Sensation", "../rpg/assets/font/sensation.ttf", 20);

	SDL_Color white = {255, 255, 255};
	int y_shift = 10;
	headingLabel =
	    std::make_shared<RpgLabel>(40, m_dialogueBox2.y + y_shift, "Press Esc to close the menu", "Sensation", white);
	y_shift += 20;
	if (fs::exists("../game_save/")) {
		for (auto &p : fs::directory_iterator("../game_save/")) {
			std::string game_name = p.path().filename().string();

			auto label =
			    std::make_shared<RpgLabel>(40, m_dialogueBox2.y + y_shift, game_name.c_str(), "Sensation", white);
			m_labels.push_back(label);

			m_buttonFucntions.insert({label, [=](RpgGame *rpgGame) {
				                          SaveGame game;
				                          game.loadGame(p.path().string());
				                          rpgGame->changeState(RpgPlayState::Instance());
			                          }});
		}
	} else {
		auto label = std::make_shared<RpgLabel>(40, m_dialogueBox2.y + y_shift, "No saved game!", "Sensation", white);
		m_labels.push_back(label);
	}
}

void RpgLoadGameState::labelPressed(LabelItem item, RpgGame *rpgGame)
{
	for (auto button : m_buttonFucntions) {
		if (button.first == item) {
			button.second(rpgGame);
		}
	}
}

RpgLoadGameState::~RpgLoadGameState() = default;

void RpgLoadGameState::Pause() {}

void RpgLoadGameState::Resume() {}

bool isEscPressed = false;
bool isLoadGameClicked = false;

void RpgLoadGameState::HandleEvents(RpgGame *rpgGame)
{
	SDL_PollEvent(&m_event);
	switch (m_event.type) {
	case SDL_QUIT:
		rpgGame->quitGame();
		break;
	case SDL_KEYDOWN:
		switch (m_event.key.keysym.sym) {
		case SDLK_ESCAPE:
			if (!isEscPressed) {
				isEscPressed = true;
				rpgGame->popState();
			}
			break;
		}
		break;
	case SDL_KEYUP:
		switch (m_event.key.keysym.sym) {
		case SDLK_ESCAPE:
			isEscPressed = false;
			break;
		}
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP: {
		int mousePosX, mousePosY;
		SDL_GetMouseState(&mousePosX, &mousePosY);

		for (int i = 0; i < m_labels.size(); i++) {
			SDL_Rect menuItemDims;
			m_labels[i]->getLabelDims(menuItemDims);

			bool insideLabel = true;

			if (mousePosX < menuItemDims.x || mousePosX > (menuItemDims.x + menuItemDims.w)
			    || mousePosY < menuItemDims.y || mousePosY > (menuItemDims.y + menuItemDims.h)) {
				insideLabel = false;
			}

			if (!insideLabel) {
				m_labels[i]->setLabelColor(m_colors[0]);
			} else {
				switch (m_event.type) {
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONUP:
					m_labels[i]->setLabelColor(m_colors[1]);
					isLoadGameClicked = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					m_labels[i]->setLabelColor(m_colors[1]);
					if (!isLoadGameClicked) {
						labelPressed(m_labels[i], rpgGame);
						isLoadGameClicked = true;
					}
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

void RpgLoadGameState::Update(RpgGame *rpgGame) {}

void RpgLoadGameState::Render(RpgGame *rpgGame)
{
	SDL_SetRenderDrawColor(RpgGame::renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(RpgGame::renderer, &m_dialogueBox1);
	SDL_SetRenderDrawColor(RpgGame::renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(RpgGame::renderer, &m_dialogueBox2);

	headingLabel->Draw();
	for (auto &l : m_labels) {
		l->Draw();
	}
	SDL_RenderPresent(RpgGame::renderer);
}
