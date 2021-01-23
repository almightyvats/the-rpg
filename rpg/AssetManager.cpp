#include "AssetManager.hpp"
#include "ecs/Components.hpp"
#include "rpg/states/RpgPlayState.hpp"

AssetManager::AssetManager(Manager *man) : manager(man) {}

AssetManager::~AssetManager() {}

void AssetManager::CreateNpc(Vector2D position, int tileSize, int mapScale, std::string id, State state)
{
	RpgGame::assets->AddTexture(id, "../rpg/assets/" + id + ".png");

	auto &npc(manager->addEntity(state));
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

void AssetManager::CreateEnemy(Vector2D position, int tileSize, int mapScale, std::string id, State state)
{
	RpgGame::assets->AddTexture(id, "../rpg/assets/enemies/" + id + ".png");

	auto &enemy(manager->addEntity(state));
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

void AssetManager::CreateProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string id, State state)
{
	auto &projectile(manager->addEntity(state));
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
                                 std::string id, SpriteSheet spriteSheet, std::string map, Vector2D pStart, State state)
{
	auto &tile(manager->addEntity(state));
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

void AssetManager::CreateInventoryItem(int srcX, int srcY, std::string id, Equipment equip, State state)
{
	auto &item(manager->addEntity(state));
	auto &inv = item.addComponent<InventoryComponent>();
	inv.equip = equip;
	item.addComponent<SpriteComponent>("icons", srcX * 32, srcY * 32, SpriteSheet(15, 32, 32, 0, 0));
	item.addGroup(RpgPlayState::groupItems);
}	

void AssetManager::AddTexture(std::string id, std::string path)
{
	m_textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string id)
{
	return m_textures[id];
}

void AssetManager::AddFont(const std::string &id, const std::string &path, const int &fontSize)
{
	auto openFontPtr = TTF_OpenFont(path.c_str(), fontSize);
	if (openFontPtr == nullptr) {
		std::cout << "Problem with open font\n";
	} else {
		std::cout << "Font loaded\n";
	}
	m_fonts.emplace(id, openFontPtr);
}

TTF_Font *AssetManager::GetFont(const std::string &id)
{
	return m_fonts[id];
}