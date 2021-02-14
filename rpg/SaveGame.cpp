#include "SaveGame.hpp"
#include "AssetManager.hpp"

#include "combat/LootGenerator.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>

namespace fs = std::filesystem;

extern Manager manager;

SaveGame::SaveGame()
    : pc_knight(PlayerCombatant("Knight", "c_knight", 0, 0, 0, 0, 0, 0, 0, 0)),
      pc_archer(PlayerCombatant("Archer", "c_archer", 0, 0, 0, 0, 0, 0, 0, 0)),
      pc_brute(PlayerCombatant("Brute", "c_brute", 0, 0, 0, 0, 0, 0, 0, 0)),
      save_game_file("")
{
	player_pos = Vector2D(0, 0);
    player_map = "";
}

SaveGame::~SaveGame() = default;

void SaveGame::NewGame()
{
	// find different sprites for player characters
	pc_knight = PlayerCombatant("Knight", "c_knight", 5, 55, 9, 8, 6, 11, 7, 7);
	pc_archer = PlayerCombatant("Archer", "c_archer", 5, 45, 11, 3, 8, 3, 13, 12);
	pc_brute = PlayerCombatant("Brute", "c_brute", 5, 75, 2, 15, 11, 8, 2, 7);

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

void SaveGame::saveCurrentGame()
{
	std::stringstream ss;

	int count = 0;
	if (!fs::exists("game_save/")) {
		fs::create_directories("game_save/");
	}

	for (auto &p : fs::directory_iterator("game_save/")) {
		count++;
	}

	std::string output_file = "game_save/saved_game_" + std::to_string(++count) + ".json";

	{
		cereal::JSONOutputArchive ar(ss);
		inventory = FetchInventory();
		items_knight = FetchItemsKnight();
		items_archer = FetchItemsArcher();
		items_brute = FetchItemsBrute();

		ar(inventory, items_knight, items_archer, items_brute, pc_knight, pc_archer, pc_brute, player_pos, player_map);
	}
	std::ofstream outFile(output_file);

	outFile << ss.rdbuf();
    
    save_game_file = output_file;
}

void SaveGame::saveCurrentGame(const std::string & output_file)
{
    std::stringstream ss;
    std::remove(output_file.c_str());

    {
		cereal::JSONOutputArchive ar(ss);
		inventory = FetchInventory();
		items_knight = FetchItemsKnight();
		items_archer = FetchItemsArcher();
		items_brute = FetchItemsBrute();

		ar(inventory, items_knight, items_archer, items_brute, pc_knight, pc_archer, pc_brute, player_pos, player_map);
	}
	std::ofstream outFile(output_file);

	outFile << ss.rdbuf();
}

void SaveGame::loadGame(const std::string &saved_game_path)
{
	std::ifstream file(saved_game_path);
	std::stringstream ss;
	if (file) {
		ss << file.rdbuf();
	} else {
		return;
	}
	{
		cereal::JSONInputArchive ir(ss);
		ir(inventory, items_knight, items_archer, items_brute, pc_knight, pc_archer, pc_brute, player_pos, player_map);
		SetInventory(inventory);
		SetItemsKnight(items_knight);
		SetItemsArcher(items_archer);
		SetItemsBrute(items_brute);
        save_game_file = saved_game_path;
	}
}
