#include "AssetManager.hpp"
#include "ecs/Components.hpp"
#include "rpg/states/RpgCombatState.hpp"
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

void AssetManager::CreateCombatant(Vector2D position, std::string sprite_name, bool player_team, State state)
{
	RpgGame::assets->AddTexture(sprite_name, "../rpg/assets/combatants/" + sprite_name + ".png");

	auto &combatant(manager->addEntity(state));
	combatant.addComponent<TransformComponent>(position.x, position.y, 100, 100, 1);

	SpriteSheet spriteSheet(1, 100, 100, 0, 0);
	auto &combatantSprite = combatant.addComponent<SpriteComponent>(sprite_name, spriteSheet,
	                                                                player_team ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
	{
		combatantSprite.addAnimation("idle_down", Animation(0, 1, 1));
		combatantSprite.defaultAnimation("idle_down");
	}
	if (player_team) {
		combatant.addGroup(RpgCombatState::groupCombatPlayerCombatants);
	} else {
		combatant.addGroup(RpgCombatState::groupCombatEnemyCombatants);
	}
}

void AssetManager::CreateProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string id,
                                    State state)
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

void getSpriteCoords(EquipmentType eqType, EquipmentMaterial eqMat, int &x, int &y)
{
	// Swords
	if (eqType == EquipmentType::sword && eqMat == EquipmentMaterial::wood) {
		x = 0;
		y = 0;
	} else if (eqType == EquipmentType::sword && eqMat == EquipmentMaterial::metal) {
		x = 0;
		y = 1;
	} else if (eqType == EquipmentType::sword && eqMat == EquipmentMaterial::gold) {
		x = 4;
		y = 0;
	} else if (eqType == EquipmentType::sword && eqMat == EquipmentMaterial::fire) {
		x = 1;
		y = 0;
	} else if (eqType == EquipmentType::sword && eqMat == EquipmentMaterial::ice) {
		x = 2;
		y = 0;
	} else if (eqType == EquipmentType::sword && eqMat == EquipmentMaterial::special) {
		x = 3;
		y = 0;
	}
	// Axes
	else if (eqType == EquipmentType::axe && eqMat == EquipmentMaterial::wood) {
		x = 14;
		y = 1;
	} else if (eqType == EquipmentType::axe && eqMat == EquipmentMaterial::metal) {
		x = 0;
		y = 2;
	} else if (eqType == EquipmentType::axe && eqMat == EquipmentMaterial::gold) {
		x = 3;
		y = 2;
	} else if (eqType == EquipmentType::axe && eqMat == EquipmentMaterial::fire) {
		x = 11;
		y = 2;
	} else if (eqType == EquipmentType::axe && eqMat == EquipmentMaterial::ice) {
		x = 10;
		y = 2;
	} else if (eqType == EquipmentType::axe && eqMat == EquipmentMaterial::special) {
		x = 9;
		y = 2;
	}
	// Bow
	else if (eqType == EquipmentType::bow && eqMat == EquipmentMaterial::wood) {
		x = 12;
		y = 4;
	} else if (eqType == EquipmentType::bow && eqMat == EquipmentMaterial::metal) {
		x = 1;
		y = 5;
	} else if (eqType == EquipmentType::bow && eqMat == EquipmentMaterial::gold) {
		x = 6;
		y = 5;
	} else if (eqType == EquipmentType::bow && eqMat == EquipmentMaterial::fire) {
		x = 9;
		y = 5;
	} else if (eqType == EquipmentType::bow && eqMat == EquipmentMaterial::ice) {
		x = 10;
		y = 5;
	} else if (eqType == EquipmentType::bow && eqMat == EquipmentMaterial::special) {
		x = 4;
		y = 5;
	}
	// Shield
	else if (eqType == EquipmentType::shield && eqMat == EquipmentMaterial::wood) {
		x = 4;
		y = 8;
	} else if (eqType == EquipmentType::shield && eqMat == EquipmentMaterial::metal) {
		x = 6;
		y = 8;
	} else if (eqType == EquipmentType::shield && eqMat == EquipmentMaterial::gold) {
		x = 12;
		y = 8;
	} else if (eqType == EquipmentType::shield && eqMat == EquipmentMaterial::fire) {
		x = 1;
		y = 9;
	} else if (eqType == EquipmentType::shield && eqMat == EquipmentMaterial::ice) {
		x = 11;
		y = 8;
	} else if (eqType == EquipmentType::shield && eqMat == EquipmentMaterial::special) {
		x = 3;
		y = 9;
	}
	// Heal
	else if (eqType == EquipmentType::heal_item && eqMat == EquipmentMaterial::wood) {
		x = 4;
		y = 9;
	} else if (eqType == EquipmentType::heal_item && eqMat == EquipmentMaterial::metal) {
		x = 5;
		y = 9;
	} else if (eqType == EquipmentType::heal_item && eqMat == EquipmentMaterial::gold) {
		x = 6;
		y = 9;
	} else if (eqType == EquipmentType::heal_item && eqMat == EquipmentMaterial::fire) {
		x = 7;
		y = 9;
	} else if (eqType == EquipmentType::heal_item && eqMat == EquipmentMaterial::ice) {
		x = 8;
		y = 9;
	} else if (eqType == EquipmentType::heal_item && eqMat == EquipmentMaterial::special) {
		x = 9;
		y = 9;
	} else {
		x = 0;
		y = 0;
	}
}

void AssetManager::CreateInventoryItem(Equipment equip, Group group, int pocketNumber)
{
	AddTexture("icons", "../rpg/assets/icons/Icon Pack_3.png");

	int srcX = 0;
	int srcY = 0;

	getSpriteCoords(equip.type(), equip.material(), srcX, srcY);

	auto &item(manager->addEntity(stateInventory));
	auto &inv = item.addComponent<InventoryComponent>(pocketNumber);
	inv.equip = equip;
	item.addComponent<SpriteComponent>("icons", srcX * 32, srcY * 32, SpriteSheet(15, 32, 32, 0, 0));
	item.addGroup(group);
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