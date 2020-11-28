#pragma once

#include <SDL.h>
#include <SDL_image.h>
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

	static void AddTile(int srcX, int srcY, int xPos, int yPos);
	static SDL_Renderer *renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent *> colliders;

  private:
	bool isRunning;
	SDL_Window *window;
};
