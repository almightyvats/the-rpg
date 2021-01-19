#pragma once

#include <vector>

#include "Combatant.hpp"
#include "PlayerCombatant.hpp"
#include "EnemyCombatant.hpp"

class Combat {
    public:
        Combat(std::vector<Combatant*> player_combatants, std::vector<Combatant*> enemy_combatants);
        Combat();

        std::vector<Combatant*> player_combatants_;
        std::vector<Combatant*> enemy_combatants_;

        void PrintStatus();
        void Initiate();

    private:
        std::vector<Combatant*> LivingPlayerCombatants();
        std::vector<Combatant*> LivingEnemyCombatants();
        void SetInitialCooldowns();

};