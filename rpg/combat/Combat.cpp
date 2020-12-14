#include "Combat.hpp"

#include <iostream>
#include <algorithm>

Combat::Combat(std::vector<Combatant*> player_combatants, std::vector<Combatant*> enemy_combatants)
{
    player_combatants_ = player_combatants;
    enemy_combatants_ = enemy_combatants;

    //init cooldowns
}

Combatant* GetSmallestCooldownCombatant(std::vector<Combatant*> combatants) {
    auto min_cd_combatant = std::min_element(combatants.begin(), combatants.end(), 
                                            [](const Combatant* a, const Combatant* b)
                                            {
                                                return a->cooldown_ < b->cooldown_;
                                            } );
    return *min_cd_combatant;
}

Combatant* Combat::ProgressToNextAttack()
{
    
    Combatant* min_cd_player = GetSmallestCooldownCombatant(player_combatants_);
    Combatant* min_cd_enemy = GetSmallestCooldownCombatant(enemy_combatants_);

    int cooldown_progression = std::min(min_cd_player->cooldown_, min_cd_enemy->cooldown_);
    Combatant* min_cd_combatant = (min_cd_player->cooldown_ <= min_cd_enemy->cooldown_) ? min_cd_player : min_cd_enemy;

    for (Combatant* player_combatant : player_combatants_) {
        player_combatant->cooldown_ -= cooldown_progression;
    }

    for (Combatant* enemy_combatant : enemy_combatants_) {
        enemy_combatant->cooldown_ -= cooldown_progression;
    }

    return min_cd_combatant;
}

void Combat::Initiate()
{
    Combatant* next_attacker = ProgressToNextAttack();

    std::cout << "It's the turn of " << next_attacker->name() << ".\n";
    next_attacker->ChooseAndPerformAction(player_combatants_, enemy_combatants_);
}