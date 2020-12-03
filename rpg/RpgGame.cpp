#include "RpgGame.hpp"
#include "AssetManager.hpp"
#include "Collision.hpp"
#include "Map.hpp"
#include "ecs/Components.hpp"

Map *map;
Manager manager;

SDL_Renderer *RpgGame::renderer = nullptr;
SDL_Event RpgGame::event;

SDL_Rect RpgGame::camera = {0, 0, 0, 0};

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
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, flags);
		if (window) {
			std::cout << "Window craeted" << std::endl;
		}

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		camera.w = w;
		camera.h = h;

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
	assets->AddTexture("fireball", "../rpg/assets/fireball_sprite.png");

	map = new Map("../rpg/assets/map/jsonsample.json", 3);
	// map = new Map("../rpg/assets/map/testmap_50_50.json", 3);
	map->LoadMap();

	player.addComponent<TransformComponent>(10 * 32 * 3, 12 * 32 * 3, 115, 75, 1);

	SpriteSheet spriteSheet(11, 75, 115, 75, 5);
	auto &playerSprite = player.addComponent<SpriteComponent>("player", spriteSheet);
	{
		playerSprite.addAnimation("idle_down", Animation(0, 1, 100));
		playerSprite.addAnimation("idle_up", Animation(1, 1, 100));
		playerSprite.addAnimation("idle_left", Animation(2, 1, 100));
		playerSprite.addAnimation("idle_right", Animation(3, 1, 100));
		playerSprite.addAnimation("walk_down", Animation(0, 7, 100));
		playerSprite.addAnimation("walk_up", Animation(1, 7, 100));
		playerSprite.addAnimation("walk_left", Animation(2, 7, 100));
		playerSprite.addAnimation("walk_right", Animation(3, 7, 100));
		playerSprite.defaultAnimation("idle_down");
	}
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player");
	player.addGroup(groupPlayers);

	assets->CreateProjectile(Vector2D(300, 300), Vector2D(0, -1), 200, 0, "fireball");
	assets->CreateProjectile(Vector2D(0, 300), Vector2D(1, 1), 200, 0, "fireball");
	assets->CreateProjectile(Vector2D(150, 300), Vector2D(1, 0), 200, 0, "fireball");
}

auto &tiles(manager.getGroup(RpgGame::groupMap));
auto &players(manager.getGroup(RpgGame::groupPlayers));
auto &colliders(manager.getGroup(RpgGame::groupColliders));
auto &projectiles(manager.getGroup(RpgGame::groupProjectiles));

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

	for (auto &t : tiles) {
		if (t->hasComponent<ColliderComponent>()) {
			SDL_Rect cCol = t->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(cCol, playerCol)) {
			std::cout << "Player hit collidable tile" << std::endl;	
			}
		}
	}

	for (auto &c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto &p : projectiles) {
		SDL_Rect cCol = p->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {
			std::cout << "Player hit" << std::endl;
			p->destroy();
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - camera.w / 2; //-half screen
	camera.y = player.getComponent<TransformComponent>().position.y - camera.h / 2; //-half screen

	if (camera.x < 0) {
		camera.x = 0;
	}
	if (camera.y < 0) {
		camera.y = 0;
	}

	int maxRight = (map->width * map->scale) - camera.w;
	int maxDown = (map->height * map->scale) - camera.h;

	if (camera.x > maxRight) {
		camera.x = maxRight;
	}
	if (camera.y > maxDown) {
		camera.y = maxDown;
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

	for (auto &p : projectiles) {
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
