#include "RpgGame.hpp"
#include "GameObject.hpp"
#include "Map.hpp"
#include "TextureManager.hpp"

GameObject *player;
Map *map;

SDL_Renderer *RpgGame::renderer = nullptr;

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
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, flags);
		if (window) {
			std::cout << "Window craeted" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer craeted" << std::endl;
		}

		isRunning = true;
	} else {
		isRunning = false;
	}

	player = new GameObject("../assets/Minotaurs/Minotaur_01/PNG Sequences/Walking/Minotaur_01_Walking_000.png", 50, 50);
	map = new Map();
}

void RpgGame::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
};
void RpgGame::update()
{
	player->Update();
};
void RpgGame::render()
{
	SDL_RenderClear(renderer);

	map->DrawMap();
	player->Render();

	// put stuff here to render
	SDL_RenderPresent(renderer);
};
void RpgGame::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "cleaned" << std::endl;
};
