#pragma once

#include <vector>

#include "Combatant.hpp"
#include "PlayerCombatant.hpp"
#include "EnemyCombatant.hpp"

class Combat {
    public:
        std::vector<PlayerCombatant> player_combatants_;
        std::vector<EnemyCombatant> enemy_combatants_;

        Combatant ProgressToNextAttack();
};