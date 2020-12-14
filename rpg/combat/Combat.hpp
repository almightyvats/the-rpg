#pragma once

#include <vector>

#include "Combatant.hpp"
#include "PlayerCombatant.hpp"
#include "EnemyCombatant.hpp"

class Combat {
    public:
        Combat(std::vector<Combatant*> player_combatants, std::vector<Combatant*> enemy_combatants);

        std::vector<Combatant*> player_combatants_;
        std::vector<Combatant*> enemy_combatants_;

        Combatant* ProgressToNextAttack();
        void Initiate();
};