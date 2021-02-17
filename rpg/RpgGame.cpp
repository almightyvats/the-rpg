#include "RpgGame.hpp"
#include "RpgSoundManager.hpp"
#include "SaveGame.hpp"
#include "rpg/states/RpgGameState.hpp"

SDL_Renderer *RpgGame::renderer = nullptr;

SDL_Rect RpgGame::camera = {0, 0, 0, 0};

bool RpgGame::isRunning = false;
SaveGame saveGame;

RpgGame::RpgGame(){};
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
			std::cout << "Window created" << std::endl;
		}
		RpgSoundManager::init();
		RpgSoundManager::addMusic("../rpg/assets/music/combat1.wav", "COMBAT1");
		RpgSoundManager::addMusic("../rpg/assets/music/menu.wav", "MENU");
		RpgSoundManager::addMusic("../rpg/assets/music/play.wav", "PLAY");
		RpgSoundManager::addSoundEffect("../rpg/assets/music/effects/explosion.wav", "EXPLOSION");
		RpgSoundManager::addSoundEffect("../rpg/assets/music/effects/rewind.wav", "REWIND");
		RpgSoundManager::addSoundEffect("../rpg/assets/music/effects/melee_single.wav", "MELEE_SINGLE");
		RpgSoundManager::addSoundEffect("../rpg/assets/music/effects/melee_multi.wav", "MELEE_MULTI");
		RpgSoundManager::addSoundEffect("../rpg/assets/music/effects/ranged_single.wav", "RANGED_SINGLE");
		RpgSoundManager::addSoundEffect("../rpg/assets/music/effects/ranged_multi.wav", "RANGED_MULTI");
		RpgSoundManager::addSoundEffect("../rpg/assets/music/effects/potion.wav", "POTION");
		RpgSoundManager::addSoundEffect("../rpg/assets/music/effects/shield.wav", "SHIELD");
		RpgSoundManager::addSoundEffect("../rpg/assets/music/effects/ignite.wav", "IGNITE");

		if (TTF_Init() < 0) {
			std::cout << "Error in loading TTF: " << TTF_GetError() << std::endl;
		} else {
			std::cout << "TTF initialised" << std::endl;
		}

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		camera.w = w;
		camera.h = h;

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Renderer created" << std::endl;
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
	Mix_CloseAudio();
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
	if (!m_states.empty()) {
		m_states.pop_back();
	}

	m_states.emplace_back(state);
	m_states.back().get().Resume();
}

void RpgGame::popState()
{
	if (!m_states.empty())
		m_states.pop_back();
	if (!m_states.empty()) {
		m_states.back().get().Resume();
	}
}