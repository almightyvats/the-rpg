#include "RpgPlayerConvoState.hpp"

RpgPlayerConvoState::RpgPlayerConvoState()
{
	m_dialogueBox.x = 25;
	m_dialogueBox.y = RpgGame::SCREEN_HEIGHT - 200;
	m_dialogueBox.w = RpgGame::SCREEN_WIDTH - 50;
	m_dialogueBox.h = 150;

	SDL_Color white = {255, 255, 255};
	auto playerDialogueLabel =
	    std::make_shared<RpgLabel>(30, m_dialogueBox.y + 10, "What's up?", "Converstation", white);

	m_labels.push_back(playerDialogueLabel);
}

RpgPlayerConvoState::~RpgPlayerConvoState() {}

void RpgPlayerConvoState::Pause() {}

void RpgPlayerConvoState::Resume() {}

void RpgPlayerConvoState::HandleEvents(RpgGame *rpgGame)
{
	SDL_PollEvent(&m_event);
	switch (m_event.type) {
	case SDL_QUIT:
		rpgGame->quitGame();
		break;
	case SDL_KEYDOWN:
		switch (m_event.key.keysym.sym) {
		case SDLK_SPACE:
			rpgGame->popState();
		}
	}
}

void RpgPlayerConvoState::Update(RpgGame *rpgGame) {}

void RpgPlayerConvoState::Render(RpgGame *rpgGame)
{
	SDL_RenderFillRect(RpgGame::renderer, &m_dialogueBox);

	for (auto &l : m_labels) {
		l->Draw();
	}
	SDL_RenderPresent(RpgGame::renderer);
}