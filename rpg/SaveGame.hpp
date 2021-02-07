#pragma once

#include "combat/PlayerCombatant.hpp"
#include "ecs/InventoryComponent.hpp"
#include "Vector2D.hpp"

class SaveGame
{
    public:
        PlayerCombatant pc_knight;
        PlayerCombatant pc_archer;
        PlayerCombatant pc_brute;

        std::vector<InventoryComponent> inventory;
        Vector2D player_pos;

        SaveGame();
        ~SaveGame();

        void NewGame();
        std::vector<Combatant*> FetchCombatants();
};