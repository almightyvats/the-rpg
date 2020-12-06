#include "Combatant.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>

bool CheckAttackHit(int hit_stat, int evade_stat, float accuracy, AttackEffect effect)
{
    float hit_evade_chance;

    if (evade_stat <= 0 && hit_stat > 0) {
        hit_evade_chance = 1.5
    } if (hit_stat <= 0) {
        hit_evade_chance = 0.5
    } else {
        hit_evade_chance = (float) hit_stat / (float) evade_stat;
        hit_evade_chance = std::min(hit_evade_chance, 1.5);
        hit_evade_chance = std::max(hit_evade_chance, 0.5)
    }

    std::srand(std::time(nullptr));

    float roll_needed = hit_evade_chance * accuracy * 1000;
    int roll = (std::rand() % 1000);
    return roll < roll_needed;
}

void Combatant::PerformAttack(Attack attack, std::vector<Combatant*> targets)
{

    CombatantStats attacker_stats = CalculateStats();

    for (Combatant* target : targets) {
        CombatantStats target_stats = target->CalculateStats();

        bool attack_hit;

        if (attack.target_type == AttackType::melee) {
            attack_hit = CheckAttackHit(attacker_stats.dexterity, target_stats.dexterity, attack.accuracy, attack.effect);
        } else {
            attack_hit = CheckAttackHit(attacker_stats.perception, target_stats.perception, attack.accuracy, attack.effect);
        }

        if (attack_hit) {
            
        } else {
            //attack_miss
        }
    }

}