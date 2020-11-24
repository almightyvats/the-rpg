#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string.h>

class RpgGame {
  public:
	RpgGame();
	~RpgGame();

	void init(std::string title, bool fullScreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

	static SDL_Renderer *renderer;
	static SDL_Event event;

  private:
	bool isRunning;
	SDL_Window *window;
};
