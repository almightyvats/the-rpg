#include "RpgGame.hpp"
#include "rpg/states/RpgGameState.hpp"
#include "sound/RpgSoundManager.hpp"

SDL_Renderer *RpgGame::renderer = nullptr;

SDL_Rect RpgGame::camera = {0, 0, 0, 0};

bool RpgGame::isRunning = false;

RpgGame::RpgGame() {}
RpgGame::~RpgGame() {}

void RpgGame::init(std::string title, bool fullScreen)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		int flags = 0;
		if (fullScreen) {
			flags = SDL_WINDOW_FULLSCREEN;
		}

		std::cout << "Initialized ..." << std::endl;
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
		                          SCREEN_HEIGHT, flags);
		if (window) {
			std::cout << "Window craeted" << std::endl;
		}
		// RpgSoundManager soundManager;
		RpgSoundManager::init();
		RpgSoundManager::addMusic("../assets/sound/sample.mp3", "SAMPLE");

		RpgSoundManager::playMusic("SAMPLE");

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		camera.w = w;
		camera.h = h;

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Renderer craeted" << std::endl;
		}

		isRunning = true;
	} else {
		isRunning = false;
	}
}

void RpgGame::handleEvents()
{
	m_states.back().get().HandleEvents(this);
};

void RpgGame::update()
{
	m_states.back().get().Update(this);
};

void RpgGame::render()
{
	m_states.back().get().Render(this);
};
void RpgGame::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	Mix_Quit();
	std::cout << "cleaned" << std::endl;
};

void RpgGame::pushState(RpgGameState &state)
{
	if (!m_states.empty()) {
		m_states.back().get().Pause(); // Implement Pause in PlayState
	}

	m_states.emplace_back(state);
}

void RpgGame::changeState(RpgGameState &state)
{
	if (!m_states.empty())
		m_states.pop_back();

	m_states.emplace_back(state);
}

void RpgGame::popState()
{
	if (!m_states.empty())
		m_states.pop_back();
	if (!m_states.empty()) {
		m_states.back().get().Resume(); // Implement Resume in PlayState
	}
}