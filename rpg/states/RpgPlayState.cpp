#include "RpgCombatState.hpp"
#include "RpgPlayerConvoState.hpp"
#include "rpg/AssetManager.hpp"
#include "rpg/Collision.hpp"
#include "rpg/Map.hpp"
#include "rpg/RpgSoundManager.hpp"
#include "rpg/SaveGame.hpp"
#include "rpg/Vector2D.hpp"
#include "rpg/ecs/Components.hpp"
#include "rpg/states/RpgStates.hpp"
#include "rpg/combat/LootGenerator.hpp"

#include <ctime>

#define ENEMY_PHASE_TIME 1.5

Map *map;
Manager manager;
const State m_state = statePlay;
extern SaveGame saveGame;

extern bool enemy_destroyed;
extern bool game_over;
bool playsession_reloadable = false;
static std::time_t last_encounter_escape;
static std::time_t last_conversation_or_combat;
Entity *enemy_encountered;

AssetManager *RpgGame::assets = new AssetManager(&manager);
SDL_Event RpgPlayState::event;

Uint8 fade = 0;
Uint8 alpha = SDL_ALPHA_OPAQUE;

auto &player(manager.addEntity(m_state));

bool godMode = false;
SDL_Keycode conamiCode[] = {SDLK_UP,    SDLK_UP,   SDLK_DOWN,  SDLK_DOWN, SDLK_LEFT,
                            SDLK_RIGHT, SDLK_LEFT, SDLK_RIGHT, SDLK_b,    SDLK_a};
bool conamiCodeInput[] = {false, false, false, false, false, false, false, false, false, false};
int conamiCodeIndex = 0;

RpgPlayState::RpgPlayState()
{
	RpgGame::assets->AddTexture("player", "../rpg/assets/playerSpriteSheet.png");
	RpgGame::assets->AddTexture("fireball", "../rpg/assets/fireball_sprite.png");

	player.addComponent<TransformComponent>(55 * 32 * 3, 4 * 32 * 3, 115, 75, 1);

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

	RpgSoundManager::resumeMusic("PLAY");
}

RpgPlayState::~RpgPlayState() = default;

auto &tiles(manager.getGroup(RpgPlayState::groupMap));
auto &players(manager.getGroup(RpgPlayState::groupPlayers));
auto &colliders(manager.getGroup(RpgPlayState::groupColliders));
auto &projectiles(manager.getGroup(RpgPlayState::groupProjectiles));
auto &npcs(manager.getGroup(RpgPlayState::groupNpcs));
auto &enemies(manager.getGroup(RpgPlayState::groupEnemies));

void RpgPlayState::Pause()
{
	player.getComponent<TransformComponent>().velocity = Vector2D(0, 0);
	player.getComponent<SpriteComponent>().play("idle_down");
}

void RpgPlayState::Resume()
{
	playsession_reloadable = true;
	RpgSoundManager::resumeMusic("PLAY");

	if (enemy_encountered != NULL) {
		if (enemy_destroyed) {
			enemy_encountered->destroy();
		} else {
			last_encounter_escape = std::time(0);
		}
		enemy_encountered = NULL;
	}
}

void RpgPlayState::LoadPlayerPos()
{
	manager.refresh(m_state);
	map = new Map(saveGame.player_map, 3);
	map->LoadMap();
	player.getComponent<TransformComponent>().position = saveGame.player_pos;
	manager.update(m_state);
}

bool CheckKonami(SDL_Keycode keyCode)
{
	// Check for special input
	if (conamiCode[conamiCodeIndex] == keyCode) {
		bool prevInputCorrect = true;
		for (int i = 0; i < conamiCodeIndex; i++) {
			prevInputCorrect &= conamiCodeInput[i];
		}
		if (prevInputCorrect) {
			conamiCodeInput[conamiCodeIndex] = true;
			conamiCodeIndex++;
		} else {
			conamiCodeIndex = 0;
		}
	} else {
		conamiCodeIndex = 0;
	}

	if (conamiCodeIndex == 0) {
		for (int i = 0; i < 10; i++) {
			conamiCodeInput[i] = false;
		}
	}
	return conamiCodeInput[9];
}

CombatArena DetermineArena()
{
	if (map->mapFilePath.compare("../rpg/assets/map/outdoor_02.json") == 0) {
		return CombatArena::stone;
	} else {
		return CombatArena::grass;
	}
}

void RpgPlayState::HandleEvents(RpgGame *rpgGame)
{
	if (SDL_PollEvent(&event) == 1) {
		switch (event.type) {
		case SDL_QUIT:
			rpgGame->quitGame();
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				rpgGame->changeState(RpgMenuState::Instance());
				break;
			case SDLK_i:
				rpgGame->changeState(RpgInventoryState::Instance());
				break;
			case SDLK_p:
				saveGame.player_map = map->mapFilePath;
				saveGame.saveCurrentGame();
				break;
			case SDLK_o:
				saveGame.player_map = map->mapFilePath;
				if (saveGame.save_game_file == "") {
					saveGame.saveCurrentGame();
				} else {
					saveGame.saveCurrentGame(saveGame.save_game_file);
				}
				break;
			}
			break;
		case SDL_KEYUP:
			if (CheckKonami(event.key.keysym.sym)) {
				godMode = !godMode;
				if (godMode) {
					std::cout << "Godmode activated" << std::endl;
				} else {
					std::cout << "Godmode deactivated" << std::endl;
				}
			}
			break;
		default:
			break;
		}
	}
}

std::string newMap;
Vector2D playerStart;

void RpgPlayState::Update(RpgGame *rpgGame)
{
	if (game_over) {
		playsession_reloadable = false;
		game_over = false;
		rpgGame->changeState(RpgMenuState::Instance());
	}

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	saveGame.player_pos = playerPos;

	manager.refresh(m_state);
	manager.update(m_state);

	for (auto &t : tiles) {
		if (t->hasComponent<ColliderComponent>()) {
			SDL_Rect cCol = t->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(cCol, playerCol)) {
				if (!godMode) {
					player.getComponent<TransformComponent>().position = playerPos;
				}
				if (t->hasComponent<DoorComponent>()) {
					auto &door = t->getComponent<DoorComponent>();
					// Door only passable if in godmode, or any combatant has reached lvl 10
					if (saveGame.pc_archer.level() >= door.minLvl || saveGame.pc_brute.level() >= door.minLvl
					    || saveGame.pc_knight.level() >= door.minLvl || godMode) {
						newMap = door.targetMap;
						playerStart = door.playerStart;
						fade = -5;
					}
				}
			}
		}
	}

	for (auto &n : npcs) {
		if (n->hasComponent<ColliderComponent>()) {
			SDL_Rect cCol = n->getComponent<ColliderComponent>().collider;

			if (Collision::AABB(cCol, playerCol)) {
				player.getComponent<TransformComponent>().position = playerPos;
				if (RpgPlayerConvoState::isPlayerReadyToTalk()) {
					rpgGame->pushState(RpgPlayerConvoState::Instance(n->getComponent<NameComponent>().name));
				}
				// talking to old man and no inventory - get some items
				if (n->hasComponent<NameComponent>()) {
					if (n->getComponent<NameComponent>().name == "unnamed old man") {
						if (saveGame.FetchInventory().size() == 0) {

							GenerateLoot(2, 4);
							GenerateBaseLoot(2, 4);
							GenerateBaseLoot(2, 4);
							GenerateBaseLoot(2, 4);
							GenerateBaseLoot(2, 4);
							GenerateBaseLoot(2, 4);
						}
					}
				}
			}
		}
	}

	for (auto &e : enemies) {
		if (e->hasComponent<TransformComponent>()) {
			Vector2D enemyPos = e->getComponent<TransformComponent>().position;

			// if the player enters a certainr range of the enemy, the enemy will follow
			if (Vector2D::Distance(playerPos, enemyPos) < 350) {
				Vector2D enemyVelocity = e->getComponent<TransformComponent>().velocity;

				enemyVelocity.x = (enemyPos.x + playerPos.x) / 2;
				enemyVelocity.y = (enemyPos.y + playerPos.y) / 2;

				enemyVelocity -= enemyPos;
				enemyVelocity = Vector2D::Normalize(enemyVelocity);

				e->getComponent<TransformComponent>().velocity = enemyVelocity;
				e->getComponent<TransformComponent>().speed = 1;
			} else {
				e->getComponent<TransformComponent>().velocity = Vector2D(0, 0);
				e->getComponent<TransformComponent>().speed = 0;
			}
		}

		if (e->hasComponent<ColliderComponent>()) {
			SDL_Rect cCol = e->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(cCol, playerCol) && std::difftime(time(0), last_encounter_escape) > ENEMY_PHASE_TIME) {
				rpgGame->pushState(RpgCombatState::Instance(saveGame.FetchCombatants(), DetermineArena()));
				enemy_encountered = e;
			}
		}
	}

	for (auto &c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) {
			player.getComponent<TransformComponent>().position = playerPos;
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
		for (auto &n : npcs) {
			n->destroy();
		}
		for (auto &p : projectiles) {
			p->destroy();
		}

		manager.refresh(m_state);
		map = new Map(newMap, 3);
		saveGame.player_map = map->mapFilePath;
		map->LoadMap();

		// Loading new map finished -> start fading in again
		fade = 5;
	}
}

void RpgPlayState::Render(RpgGame *rpgGame)
{
	SDL_RenderClear(rpgGame->renderer);

	for (auto &t : tiles) {
		t->draw(alpha, m_state);
	}

	for (auto &c : colliders) {
		c->draw(alpha, m_state);
	}

	for (auto &n : npcs) {
		n->draw(alpha, m_state);
	}

	for (auto &p : players) {
		p->draw(alpha, m_state);
	}

	for (auto &p : projectiles) {
		p->draw(alpha, m_state);
	}

	for (auto &e : enemies) {
		e->draw(alpha, m_state);
	}

	SDL_RenderPresent(rpgGame->renderer);
}
