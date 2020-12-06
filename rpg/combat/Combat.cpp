#include "Combat.hpp"


Combatant GetSmallestCooldownCombatant(std::vector<Combatant> combatants) {
    return std::min_element(player_combatants_.begin(), player_combatants_.end(), 
                                            [](const Combatant &a, const Combatant &b)
                                            {
                                                return a.cooldown_ < b.cooldown_;
                                            } );
}

Combatant Combat::ProgressToNextAttack() {
    
    auto min_cd_player = GetSmallestCooldownCombatant(player_combatants_);
    auto min_cd_enemy = GetSmallestCooldownCombatant(enemy_combatants_);

    if (min_cd_player.cooldown_ < min_cd_enemy.cooldown_) {
        return min_cd_player;
    }
}