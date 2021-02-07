#include "SaveGame.hpp"

#include "combat/LootGenerator.hpp"

#include <cstdlib>
#include <ctime>

SaveGame::SaveGame() :
pc_knight(PlayerCombatant("Knight", "c_player", 0, 0, 0, 0, 0, 0, 0, 0)),
pc_archer(PlayerCombatant("Archer", "c_player", 0, 0, 0, 0, 0, 0, 0, 0)),
pc_brute(PlayerCombatant("Brute", "c_player", 0, 0, 0, 0, 0, 0, 0, 0))
{
    player_pos = Vector2D(0,0);
}

SaveGame::~SaveGame() = default;

void SaveGame::NewGame()
{
    //find different sprites for player characters
    pc_knight = PlayerCombatant("Knight", "c_player", 5, 55, 9, 8, 6, 11, 7, 7);
    pc_archer = PlayerCombatant("Archer", "c_player", 5, 45, 11, 3, 8, 3, 13, 12);
    pc_brute = PlayerCombatant("Brute", "c_player", 5, 75, 2, 15, 11, 8, 2, 7);

    //Init items 

    //test items:
    std::srand(std::time(nullptr));
    pc_knight.AddEquipment(GenerateLoot(3,5));
    pc_knight.AddEquipment(GenerateLoot(3,5));
    pc_archer.AddEquipment(GenerateLoot(3,5));
    pc_archer.AddEquipment(GenerateLoot(3,5));
    pc_brute.AddEquipment(GenerateLoot(3,5));
    pc_brute.AddEquipment(GenerateLoot(3,5));
}

std::vector<Combatant*> SaveGame::FetchCombatants()
{
    std::vector<Combatant*> combatants;
    combatants.push_back(&pc_knight);
    combatants.push_back(&pc_archer);
    combatants.push_back(&pc_brute);
    return combatants;
}