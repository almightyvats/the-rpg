#include "AssetManager.hpp"
#include "ecs/Components.hpp"
#include "rpg/states/RpgPlayState.hpp"

AssetManager::AssetManager(Manager *man) : manager(man) {}

AssetManager::~AssetManager() {}

void AssetManager::CreateNpc(Vector2D position, int tileSize, int mapScale, std::string id)
{
	RpgPlayState::assets->AddTexture(id, "../rpg/assets/" + id + ".png");

	auto &npc(manager->addEntity());
	npc.addComponent<TransformComponent>(position.x * tileSize * mapScale, position.y * tileSize * mapScale, 115, 115,
	                                     1);

	SpriteSheet spriteSheet(30, 100, 100, 0, 0);
	auto &npcSprite = npc.addComponent<SpriteComponent>(id, spriteSheet);
	{
		npcSprite.addAnimation("idle_down", Animation(0, 30, 100));
		npcSprite.defaultAnimation("idle_down");
	}
	npc.addComponent<ColliderComponent>("NPC");
	npc.addGroup(RpgPlayState::groupNpcs);
}

void AssetManager::CreateEnemy(Vector2D position, int tileSize, int mapScale, std::string id)
{
	RpgPlayState::assets->AddTexture(id, "../rpg/assets/enemies/" + id + ".png");

	auto &enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(position.x * tileSize * mapScale, position.y * tileSize * mapScale, 115, 115,
	                                     1);

	SpriteSheet spriteSheet(12, 124, 100, 0, 0);
	auto &enemySprite = enemy.addComponent<SpriteComponent>(id, spriteSheet);
	{
		enemySprite.addAnimation("idle_down", Animation(0, 12, 100));
		enemySprite.defaultAnimation("idle_down");
	}
	enemy.addComponent<ColliderComponent>("ENEMY");
	enemy.addGroup(RpgPlayState::groupEnemies);
}

void AssetManager::CreateProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string id)
{
	auto &projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(position.x, position.y, 32, 32, 5);

	SpriteSheet spriteSheet(6, 512, 512, 0, 0);
	auto &spriteComponent = projectile.addComponent<SpriteComponent>(id, spriteSheet);
	{
		spriteComponent.addAnimation("burn", Animation(0, 6, 100));
		spriteComponent.defaultAnimation("burn");
	}

	projectile.addComponent<ProjectileComponent>(range, speed, velocity);
	projectile.addComponent<ColliderComponent>("projectile", 50, 50, position.x, position.y, 85, 1, id);
	projectile.addGroup(RpgPlayState::groupProjectiles);
}

void AssetManager::CreateMapTile(int srcX, int srcY, int destX, int destY, int tsize, int tscale, bool withCollision,
                                 std::string id, SpriteSheet spriteSheet, std::string map, Vector2D pStart)
{
	auto &tile(manager->addEntity());
	tile.addComponent<TransformComponent>(destX, destY, tsize, tsize, tscale);
	tile.addComponent<SpriteComponent>(id, srcX, srcY, spriteSheet);

	if (withCollision) {
		tile.addComponent<ColliderComponent>("MapTile");
	}
	if (map != "") {
		tile.addComponent<DoorComponent>(map, pStart);
	}
	tile.addGroup(RpgPlayState::groupMap);
}

void AssetManager::AddTexture(std::string id, std::string path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string id)
{
	return textures[id];
}