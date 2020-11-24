#include "RpgGame.hpp"
#include "Collision.hpp"
#include "Components.hpp"
#include "Map.hpp"

Map *map;
Manager manager;

SDL_Renderer *RpgGame::renderer = nullptr;
SDL_Event RpgGame::event;

std::vector<ColliderComponent *> RpgGame::colliders;

auto &player(manager.addEntity());
auto &wall(manager.addEntity());

enum groupLabels : std::size_t { groupMap, groupPlayers, groupEnemies, groupColliders };

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

	map = new Map();

	Map::LoadMap("../maps/test.map.txt", 16, 16);

	player.addComponent<TransformComponent>(0, 0, 115, 75, 1);
	player.addComponent<SpriteComponent>("../assets/playerSpriteSheet.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player");
	player.addGroup(groupPlayers);

	wall.addComponent<TransformComponent>(200, 200, 300, 20, 1);
	wall.addComponent<SpriteComponent>("../assets/dirt.png");
	wall.addComponent<ColliderComponent>("Wall");
	wall.addGroup(groupMap);
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

	for (auto cc : colliders) {
		if (Collision::AABB(player.getComponent<ColliderComponent>(), *cc)) {

			//	player.getComponent<TransformComponent>().velocity * -1;
			std::cout << "Wall Hit!" << std::endl;
		}
	}
};

auto &tiles(manager.getGroup(groupMap));
auto &players(manager.getGroup(groupPlayers));
auto &enemies(manager.getGroup(groupEnemies));

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

void RpgGame::AddTile(int id, int x, int y)
{
	auto &tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);
	tile.addGroup(groupMap);
}
