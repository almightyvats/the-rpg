#include "RpgPlayerConvoState.hpp"

RpgPlayerConvoState::RpgPlayerConvoState()
{
	m_dialogueBox1.x = 25;
	m_dialogueBox1.y = RpgGame::SCREEN_HEIGHT - 200;
	m_dialogueBox1.w = RpgGame::SCREEN_WIDTH - 50;
	m_dialogueBox1.h = 150;

	m_dialogueBox2.x = 30;
	m_dialogueBox2.y = RpgGame::SCREEN_HEIGHT - 190;
	m_dialogueBox2.w = RpgGame::SCREEN_WIDTH - 60;
	m_dialogueBox2.h = 130;

	SDL_Color white = {255, 255, 255};
	auto playerDialogueLabel = std::make_shared<RpgLabel>(
	    40, m_dialogueBox2.y + 10,
	    "Do you want to have a conversation? Press C to continue or press SPACE and run away in under 5 seconds.",
	    "Converstation", white);

	m_labels.push_back(playerDialogueLabel);

	m_timer = std::make_unique<RpgTimer>();
}

RpgPlayerConvoState::~RpgPlayerConvoState() {}

void RpgPlayerConvoState::Pause() {}

void RpgPlayerConvoState::Resume() {}

bool isSpacePressed = false;
bool isKey_cPressed = false;

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
			if (!isSpacePressed) {
				isSpacePressed = true;
				rpgGame->popState();
				m_timer->setTimeout([this]() mutable { this->setPlayerReadyToTalk(true); }, 5000);
			}
			break;
		case SDLK_c:
			if (!isKey_cPressed) {
				isKey_cPressed = true;
			}
			break;
		}
		break;
	case SDL_KEYUP:
		switch (m_event.key.keysym.sym) {
		case SDLK_SPACE:
			isSpacePressed = false;
			break;
		case SDLK_c:
			isKey_cPressed = false;
			break;
		}
		break;
	default:
		break;
	}
}

void RpgPlayerConvoState::Update(RpgGame *rpgGame) {}

void RpgPlayerConvoState::Render(RpgGame *rpgGame)
{
	SDL_SetRenderDrawColor(RpgGame::renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(RpgGame::renderer, &m_dialogueBox1);
	SDL_SetRenderDrawColor(RpgGame::renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(RpgGame::renderer, &m_dialogueBox2);

	for (auto &l : m_labels) {
		l->Draw();
	}
	SDL_RenderPresent(RpgGame::renderer);
}

bool RpgPlayerConvoState::isPlayerReadyToTalk()
{
	return m_readyToTalk;
}

void RpgPlayerConvoState::setPlayerReadyToTalk(bool talkOrNot)
{
	m_readyToTalk = talkOrNot;
}
