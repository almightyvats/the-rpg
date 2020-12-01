#include "RpgGame.hpp"
#include "AssetManager.hpp"
#include "Collision.hpp"
#include "Map.hpp"
#include "ecs/Components.hpp"
#include "AssetManager.hpp"

Map *map;
Manager manager;

SDL_Renderer *RpgGame::renderer = nullptr;
SDL_Event RpgGame::event;

SDL_Rect RpgGame::camera = {0, 0, 800, 640};

AssetManager *RpgGame::assets = new AssetManager(&manager);

bool RpgGame::isRunning = false;

auto &player(manager.addEntity());

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
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 800, flags);
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

	assets->AddTexture("player", "../rpg/assets/playerSpriteSheet.png");
	assets->AddTexture("mapSprites", "../rpg/assets/map/pipoya_tileset.png");

	map = new Map("../rpg/assets/map/testmap_10_10.json", 3);
	map->LoadMap("mapSprites");

	// Map::LoadMap("../maps/jsonsample.json");
	// Map::LoadMap("../maps/testmap_5_5.json");

	// Map::LoadMap("../maps/testmap_50_50.json");

	player.addComponent<TransformComponent>(5 * 32, 5 * 32, 115, 75, 1);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player");
	player.addGroup(groupPlayers);
}

auto &tiles(manager.getGroup(RpgGame::groupMap));
auto &players(manager.getGroup(RpgGame::groupPlayers));
auto &colliders(manager.getGroup(RpgGame::groupColliders));

void RpgGame::handleEvents()
{

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
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	for (auto &c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - 400; //-half screen
	camera.y = player.getComponent<TransformComponent>().position.y - 320; //-half screen

	if (camera.x < 0) {
		camera.x = 0;
	}
	if (camera.y < 0) {
		camera.y = 0;
	}
	if (camera.x > camera.w) {
		camera.x = camera.w;
	}
	if (camera.y > camera.h) {
		camera.y = camera.h;
	}
};

void RpgGame::render()
{
	SDL_RenderClear(renderer);

	for (auto &t : tiles) {
		t->draw();
	}

	for (auto &c : colliders) {
		c->draw();
	}

	for (auto &p : players) {
		p->draw();
	}
	SDL_RenderPresent(renderer);
};
void RpgGame::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "cleaned" << std::endl;
};
