#include "AssetManager.hpp"
#include "ecs/Components.hpp"

AssetManager::AssetManager(Manager *man) : manager(man) {}

AssetManager::~AssetManager() {}

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
	projectile.addGroup(RpgGame::groupProjectiles);
}

void AssetManager::AddTexture(std::string id, std::string path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string id)
{
	return textures[id];
}