#include "RpgPlayerConvoState.hpp"
#include <fstream>

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
    m_playerDialogueLabel = std::make_shared<RpgLabel>(
	    40, m_dialogueBox2.y + 10,
	    "Press space to continue",
	    "Conversation", white);
}

RpgPlayerConvoState::~RpgPlayerConvoState() {}

void RpgPlayerConvoState::Pause() {}

void RpgPlayerConvoState::Resume() {}

bool isSpacePressed = false;
int current_count = 0;

void RpgPlayerConvoState::NewConversation(std::string npcName)
{
	SDL_Color white = {255, 255, 255};
    m_playerDialogueLabel = std::make_shared<RpgLabel>(
	    40, m_dialogueBox2.y + 10,
	    "Press space to continue",
	    "Conversation", white);

	m_isPlayerTalking = false;
    m_scriptPath = "../rpg/assets/scripts/" + npcName + ".txt";

    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(m_scriptPath);

    while (std::getline(myfile, line))
        ++number_of_lines;

	m_totalLines = number_of_lines;

	m_timer = std::make_unique<RpgTimer>();

	current_count = 0;
	isSpacePressed = false;
}

void RpgPlayerConvoState::HandleEvents(RpgGame *rpgGame)
{
	if (SDL_PollEvent(&m_event) == 1) {
		switch (m_event.type) {
		case SDL_QUIT:
			rpgGame->quitGame();
			break;
		case SDL_KEYDOWN:
			switch (m_event.key.keysym.sym) {
			case SDLK_SPACE:
				if (!isSpacePressed) {
					isSpacePressed = true;
					m_isPlayerTalking = true;
					if (current_count == m_totalLines) {
						rpgGame->popState();
						setPlayerReadyToTalk(false);
						m_timer->setTimeout([this]() mutable { this->setPlayerReadyToTalk(true); }, 5000);
					} else {
						current_count++;
					}
				}
				break;
			}
			break;
		case SDL_KEYUP:
			switch (m_event.key.keysym.sym) {
			case SDLK_SPACE:
				isSpacePressed = false;
				break;
			}
			break;
		default:
			break;
		}
	}
}

void RpgPlayerConvoState::Update(RpgGame *rpgGame) {

	if (m_isPlayerTalking)
    {
		std::string line;
        std::ifstream myfile(m_scriptPath);
		for (int i = 0; i < current_count; ++i) {
            std::getline(myfile, line);
		}
		m_playerDialogueLabel->setLabelText("Conversation", line);
	}
}

void RpgPlayerConvoState::Render(RpgGame *rpgGame)
{
	SDL_SetRenderDrawColor(RpgGame::renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(RpgGame::renderer, &m_dialogueBox1);
	SDL_SetRenderDrawColor(RpgGame::renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(RpgGame::renderer, &m_dialogueBox2);

    m_playerDialogueLabel->Draw();
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
