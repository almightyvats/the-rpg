#include "RpgGame.hpp"

RpgGame *menu = nullptr;

int main(int argc, char *argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	menu = new RpgGame();
	menu->init("The ultimate one and only RPG", false);

	while (menu->running()) {
		frameStart = SDL_GetTicks();

		menu->handleEvents();
		menu->update();
		menu->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	menu->clean();

	return 0;
}