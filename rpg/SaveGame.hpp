#pragma once

#include "Vector2D.hpp"
#include "combat/PlayerCombatant.hpp"
#include "ecs/InventoryComponent.hpp"
#include <fstream>

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

class SaveGame {
  private:
	void UpdateCombatantEquipment();

  public:
	PlayerCombatant pc_knight;
	PlayerCombatant pc_archer;
	PlayerCombatant pc_brute;

	std::vector<InventoryComponent> inventory;
	std::vector<InventoryComponent> items_knight;
	std::vector<InventoryComponent> items_archer;
	std::vector<InventoryComponent> items_brute;
	Vector2D player_pos;
	std::string player_map;

	SaveGame();
	~SaveGame();

	void NewGame();
	std::vector<Combatant *> FetchCombatants();
	std::vector<InventoryComponent> FetchInventory();
	void SetInventory(std::vector<InventoryComponent> items);
	std::vector<InventoryComponent> FetchItemsKnight();
	void SetItemsKnight(std::vector<InventoryComponent> items);
	std::vector<InventoryComponent> FetchItemsArcher();
	void SetItemsArcher(std::vector<InventoryComponent> items);
	std::vector<InventoryComponent> FetchItemsBrute();
	void SetItemsBrute(std::vector<InventoryComponent> items);
	template <class Archive>
	void serialize(Archive &archive)
	{
		archive(CEREAL_NVP(inventory), CEREAL_NVP(items_knight), CEREAL_NVP(items_archer), CEREAL_NVP(items_brute),
		        CEREAL_NVP(pc_knight), CEREAL_NVP(pc_archer), CEREAL_NVP(pc_brute));
	}

	void saveCurrentGame();
	void loadGame(const std::string &saved_game_path);
};