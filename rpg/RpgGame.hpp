#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <future>
#include <iostream>
#include <string.h>
#include <vector>

class ColliderComponent;

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
	static bool isRunning;
	static SDL_Rect camera;

	enum groupLabels : std::size_t { groupMap, groupPlayers, groupColliders };

  private:
	SDL_Window *window;
};
