#include "RpgGame.hpp"
#include "Collision.hpp"
#include "Map.hpp"
#include "ecs/Components.hpp"

Map *map;
Manager manager;

SDL_Renderer *RpgGame::renderer = nullptr;
SDL_Event RpgGame::event;

SDL_Rect RpgGame::camera = {0, 0, 800, 640};

std::vector<ColliderComponent *> RpgGame::colliders;

bool RpgGame::isRunning = false;

auto &player(manager.addEntity());
// auto &wall(manager.addEntity());

const std::string mapFile = "../rpg/assets/map/pipoya_tileset.png";

enum groupLabels : std::size_t { groupMap, groupPlayers, groupEnemies, groupColliders };

auto &tiles(manager.getGroup(groupMap));
auto &players(manager.getGroup(groupPlayers));
auto &enemies(manager.getGroup(groupEnemies));

RpgGame::RpgGame() {}
RpgGame::~RpgGame() {}

// static void LoadMapAsync(std::string mapName)
//{
//	Map::LoadMap(mapName);
//}

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

	map = new Map();

	// Map::LoadMap("../maps/jsonsample.json");
	// Map::LoadMap("../maps/testmap_5_5.json");

	// m_futures.push_back(std::async(std::launch::async, LoadMapAsync, "../maps/testmap_50_50.json"));
	Map::LoadMap("../rpg/assets/map/testmap_10_10.json");
	// Map::LoadMap("../maps/testmap_50_50.json");

	player.addComponent<TransformComponent>(5 * 32, 5 * 32, 115, 75, 1);
	player.addComponent<SpriteComponent>("../rpg/assets/playerSpriteSheet.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player");
	player.addGroup(groupPlayers);

	// wall.addComponent<TransformComponent>(200, 200, 300, 20, 1);
	// wall.addComponent<SpriteComponent>("../assets/dirt.png");
	// wall.addComponent<ColliderComponent>("Wall");
	// wall.addGroup(groupMap);
}

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
	manager.refresh();
	manager.update();

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

	// Vector2D playerVelocity = player.getComponent<TransformComponent>().velocity;
	// int playerSpeed = player.getComponent<TransformComponent>().speed;

	// for (auto t : tiles) {
	// 	t->getComponent<TileComponent>().destRect.x += (-playerVelocity.x * playerSpeed);
	// 	t->getComponent<TileComponent>().destRect.y += (-playerVelocity.y * playerSpeed);
	// }

	// for (auto cc : colliders) {
	// 	if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc)) {

	// 		//	player.getComponent<TransformComponent>().velocity * -1;
	// 		// std::cout << "Wall Hit!" << std::endl;
	// 	}
	// }
};

void RpgGame::render()
{
	SDL_RenderClear(renderer);

	for (auto &t : tiles) {
		t->draw();
	}
	for (auto &p : players) {
		p->draw();
	}
	for (auto &e : enemies) {
		e->draw();
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

void RpgGame::AddTile(int srcX, int srcY, int xPos, int yPos)
{
	auto &tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, mapFile);
	tile.addGroup(groupMap);
}
