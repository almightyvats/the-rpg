#include "RpgPlayState.hpp"
#include "RpgMenuState.hpp"
#include "rpg/AssetManager.hpp"
#include "rpg/Collision.hpp"
#include "rpg/Map.hpp"
#include "rpg/ecs/Components.hpp"
#include "rpg/sound/RpgSoundManager.hpp"

Map *map;
Manager manager;
AssetManager *RpgPlayState::assets = new AssetManager(&manager);
SDL_Event RpgPlayState::event;

Uint8 fade = 0;
Uint8 alpha = SDL_ALPHA_OPAQUE;

auto &player(manager.addEntity());

RpgPlayState::RpgPlayState()
{
	assets->AddTexture("player", "../rpg/assets/playerSpriteSheet.png");
	assets->AddTexture("fireball", "../rpg/assets/fireball_sprite.png");

	map = new Map("../rpg/assets/map/jsonsample.json", 3);
	// map = new Map("../rpg/assets/map/testmap_50_50.json", 3);
	map->LoadMap();

	player.addComponent<TransformComponent>(30 * 32 * 3, 27 * 32 * 3, 115, 75, 1);

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

	RpgSoundManager::playMusic("PLAY");
	RpgSoundManager::addSoundEffect("../rpg/assets/music/explosion.wav", "Explosion");
	RpgSoundManager::addSoundEffect("../rpg/assets/music/rewind.wav", "Rewind");
}

RpgPlayState::~RpgPlayState() = default;

auto &tiles(manager.getGroup(RpgPlayState::groupMap));
auto &players(manager.getGroup(RpgPlayState::groupPlayers));
auto &colliders(manager.getGroup(RpgPlayState::groupColliders));
auto &projectiles(manager.getGroup(RpgPlayState::groupProjectiles));

void RpgPlayState::Pause()
{
	RpgSoundManager::pauseMusic();
}

void RpgPlayState::Resume()
{
	RpgSoundManager::resumeMusic("PLAY");
}

void RpgPlayState::HandleEvents(RpgGame *rpgGame)
{
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		rpgGame->quitGame();
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_SPACE:
			rpgGame->changeState(RpgMenuState::Instance());
			break;
		case SDLK_e:
			RpgSoundManager::playEffect("Explosion");
			break;
		case SDLK_r:
			RpgSoundManager::playEffect("Rewind");
			break;
		}
		break;

	default:
		break;
	}
}

std::string newMap;
Vector2D playerStart;

void RpgPlayState::Update(RpgGame *rpgGame)
{

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	for (auto &t : tiles) {
		if (t->hasComponent<ColliderComponent>()) {
			SDL_Rect cCol = t->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(cCol, playerCol)) {
				player.getComponent<TransformComponent>().position = playerPos;

				if (t->hasComponent<DoorComponent>()) {
					auto &door = t->getComponent<DoorComponent>();
					newMap = door.targetMap;
					playerStart = door.playerStart;
					fade = -5;
				}
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

	rpgGame->camera.x = player.getComponent<TransformComponent>().position.x - rpgGame->camera.w / 2; //-half screen
	rpgGame->camera.y = player.getComponent<TransformComponent>().position.y - rpgGame->camera.h / 2; //-half screen

	if (rpgGame->camera.x < 0) {
		rpgGame->camera.x = 0;
	}
	if (rpgGame->camera.y < 0) {
		rpgGame->camera.y = 0;
	}

	int maxRight = (map->width * map->scale) - rpgGame->camera.w;
	int maxDown = (map->height * map->scale) - rpgGame->camera.h;

	if (rpgGame->camera.x > maxRight) {
		rpgGame->camera.x = maxRight;
	}
	if (rpgGame->camera.y > maxDown) {
		rpgGame->camera.y = maxDown;
	}

	alpha += fade;

	if (alpha >= SDL_ALPHA_OPAQUE) {
		alpha = SDL_ALPHA_OPAQUE;
		fade = 0;
	}
	if (alpha <= SDL_ALPHA_TRANSPARENT) {
		alpha = SDL_ALPHA_TRANSPARENT;
		fade = 0;
	}

	// Player out of screen -> start fading out
	if ((playerPos.x < rpgGame->camera.x) || (playerPos.y < rpgGame->camera.y)
	    || (playerPos.x > rpgGame->camera.x + rpgGame->camera.w)
	    || (playerPos.y > rpgGame->camera.y + rpgGame->camera.h)) {
		fade = -5;
	}

	// Totally faded out -> load new map, reset player position, etc
	if (alpha == SDL_ALPHA_TRANSPARENT) {
		rpgGame->camera.x = 0;
		rpgGame->camera.y = 0;

		player.getComponent<TransformComponent>().position.x = playerStart.x * 32 * 3;
		player.getComponent<TransformComponent>().position.y = playerStart.y * 32 * 3;

		for (auto &t : tiles) {
			t->destroy();
		}

		manager.refresh();
		map = new Map(newMap, 3);
		map->LoadMap();

		// Loading new map finished -> start fading in again
		fade = 5;
	}
}

void RpgPlayState::Render(RpgGame *rpgGame)
{
	SDL_RenderClear(rpgGame->renderer);

	for (auto &t : tiles) {
		t->draw(alpha);
	}

	for (auto &c : colliders) {
		c->draw(alpha);
	}

	for (auto &p : players) {
		p->draw(alpha);
	}

	for (auto &p : projectiles) {
		p->draw(alpha);
	}

	SDL_RenderPresent(rpgGame->renderer);
}
