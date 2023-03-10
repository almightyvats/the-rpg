#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "AssetManager.hpp"

class ColliderComponent;
class RpgGameState;

class RpgGame {
  public:
	RpgGame();
	~RpgGame();

	void init(std::string title, bool fullScreen);

	void pushState(RpgGameState &state);
	void changeState(RpgGameState &state);
	void popState();

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; }
	void quitGame() { isRunning = false; }

	static SDL_Renderer *renderer;
	static bool isRunning;
	static SDL_Rect camera;
	inline static const int SCREEN_HEIGHT = 768;
	inline static const int SCREEN_WIDTH = 1024;

	static AssetManager *assets;

  private:
	SDL_Window *window{};
	std::vector<std::reference_wrapper<RpgGameState>> m_states;
};
