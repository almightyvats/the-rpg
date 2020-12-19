#include "rpg/RpgGame.hpp"
#include "rpg/states/RpgMenuState.hpp"
#include "rpg/states/RpgPlayState.hpp"

RpgGame *game = nullptr;

int main(int argc, char *argv[])
{
	const int FPS = 200;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new RpgGame();
	game->init("The ultimate one and only RPG", false);

	game->changeState(RpgMenuState::Instance());

	while (game->running()) {
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	return 0;
}