#include "SaveGame.hpp"
#include "AssetManager.hpp"

#include "combat/LootGenerator.hpp"

#include <cstdlib>
#include <ctime>

extern Manager manager;

SaveGame::SaveGame()
    : pc_knight(PlayerCombatant("Knight", "c_player", 0, 0, 0, 0, 0, 0, 0, 0)),
      pc_archer(PlayerCombatant("Archer", "c_player", 0, 0, 0, 0, 0, 0, 0, 0)),
      pc_brute(PlayerCombatant("Brute", "c_player", 0, 0, 0, 0, 0, 0, 0, 0))
{
	player_pos = Vector2D(0, 0);
}

SaveGame::~SaveGame() = default;

void SaveGame::NewGame()
{
	// find different sprites for player characters
	pc_knight = PlayerCombatant("Knight", "c_player", 5, 55, 9, 8, 6, 11, 7, 7);
	pc_archer = PlayerCombatant("Archer", "c_player", 5, 45, 11, 3, 8, 3, 13, 12);
	pc_brute = PlayerCombatant("Brute", "c_player", 5, 75, 2, 15, 11, 8, 2, 7);

	// Init items

	// test items:
	if (manager.getGroup(RpgPlayState::groupItems).size() == 0) {
		std::srand(std::time(nullptr));
		pc_knight.AddEquipment(GenerateLoot(3, 5));
		pc_knight.AddEquipment(GenerateLoot(3, 5));
		pc_archer.AddEquipment(GenerateLoot(3, 5));
		pc_archer.AddEquipment(GenerateLoot(3, 5));
		pc_brute.AddEquipment(GenerateLoot(3, 5));
		pc_brute.AddEquipment(GenerateLoot(3, 5));
	}
}

std::vector<Combatant *> SaveGame::FetchCombatants()
{
    UpdateCombatantEquipment();
	std::vector<Combatant *> combatants;
	combatants.push_back(&pc_knight);
	combatants.push_back(&pc_archer);
	combatants.push_back(&pc_brute);
	return combatants;
}

std::vector<InventoryComponent> FetchItems(Group group)
{
	std::vector<InventoryComponent> result;

	auto &invItems(manager.getGroup(group));

	for (auto &entity : invItems) {
		if (entity->hasComponent<InventoryComponent>()) {
			result.emplace_back(entity->getComponent<InventoryComponent>());
		}
	}

	return result;
}

std::vector<InventoryComponent> SaveGame::FetchInventory()
{
	return FetchItems(RpgPlayState::groupItems);
}

std::vector<InventoryComponent> SaveGame::FetchItemsKnight()
{
	return FetchItems(RpgPlayState::groupEquipmentCombatant1);
}

std::vector<InventoryComponent> SaveGame::FetchItemsArcher()
{
	return FetchItems(RpgPlayState::groupEquipmentCombatant2);
}

std::vector<InventoryComponent> SaveGame::FetchItemsBrute()
{
	return FetchItems(RpgPlayState::groupEquipmentCombatant3);
}

void DeleteItems(Group group)
{
	auto &invItems(manager.getGroup(group));
	for (auto item : invItems) {
		item->destroy();
	}
}

void SetItems(Group group, std::vector<InventoryComponent> items)
{
	for (auto item : items) {
		RpgGame::assets->CreateInventoryItem(item.equip, group, item.pocketNumber);
	}
}

void SaveGame::SetInventory(std::vector<InventoryComponent> items)
{
	DeleteItems(RpgPlayState::groupItems);
	SetItems(RpgPlayState::groupItems, items);
}

void SaveGame::SetItemsKnight(std::vector<InventoryComponent> items)
{
	DeleteItems(RpgPlayState::groupEquipmentCombatant1);
	SetItems(RpgPlayState::groupEquipmentCombatant1, items);
}

void SaveGame::SetItemsArcher(std::vector<InventoryComponent> items)
{
	DeleteItems(RpgPlayState::groupEquipmentCombatant2);
	SetItems(RpgPlayState::groupEquipmentCombatant2, items);
}

void SaveGame::SetItemsBrute(std::vector<InventoryComponent> items)
{
	DeleteItems(RpgPlayState::groupEquipmentCombatant3);
	SetItems(RpgPlayState::groupEquipmentCombatant3, items);
}

void SaveGame::UpdateCombatantEquipment()
{
    pc_knight.ClearEquipment();
    for (auto &item : FetchItemsKnight()) {
        pc_knight.AddEquipment(item.equip);
    }

    pc_archer.ClearEquipment();
    for (auto &item : FetchItemsArcher()) {
        pc_archer.AddEquipment(item.equip);
    }

    pc_brute.ClearEquipment();
    for (auto &item : FetchItemsBrute()) {
        pc_brute.AddEquipment(item.equip);
    }
}
