#include "Combatant.hpp"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#define MIN_HIT_EVADE_FACTOR 0.5f
#define MAX_HIT_EVADE_FACTOR 1.5f
#define MIN_LUCK_FACTOR 0.2f
#define MAX_LUCK_FACTOR 2.0f

std::string Combatant::state_string() {
    switch (state_)
    {
    case CombatantState::normal:
        return "normal";
    case CombatantState::dead:
        return "dead";
    default:
        return "unknown";
    }
}

bool CheckAttackHit(int hit_stat, int evade_stat, float accuracy, AttackEffect effect)
{
    float hit_evade_factor;

    if (evade_stat <= 0 && hit_stat > 0) {
        hit_evade_factor = MAX_HIT_EVADE_FACTOR;
    } else if (hit_stat <= 0) {
        hit_evade_factor = MIN_HIT_EVADE_FACTOR;
    } else {
        hit_evade_factor = (float) hit_stat / (float) evade_stat;
        hit_evade_factor = std::min(hit_evade_factor, MAX_HIT_EVADE_FACTOR);
        hit_evade_factor = std::max(hit_evade_factor, MIN_HIT_EVADE_FACTOR);
    }

    std::srand(std::time(nullptr));

    float roll_needed = hit_evade_factor * accuracy * 1000;
    int roll = (std::rand() % 1000);
    return roll < roll_needed;
}

bool CheckCritHit(int attacker_luck, int defender_luck, float crit_chance, AttackEffect effect)
{
    float luck_factor;

    if (defender_luck <= 0 && attacker_luck > 0) {
        luck_factor = MAX_LUCK_FACTOR;
    } else if (attacker_luck <= 0) {
        luck_factor = MIN_LUCK_FACTOR;
    } else {
        luck_factor = (float) attacker_luck / (float) defender_luck;
        luck_factor = std::min(luck_factor, MAX_LUCK_FACTOR);
        luck_factor = std::max(luck_factor, MIN_LUCK_FACTOR);
    }

    std::srand(std::time(nullptr));

    float roll_needed = luck_factor * crit_chance * 1000;
    int roll = (std::rand() % 1000);
    return roll < roll_needed;
}

int CalculateAttackDamage(int attacker_strength, int defender_defense, int damage, AttackEffect effect)
{
    float strength_defense_factor = (float) attacker_strength / (float) defender_defense;
    return (int) (strength_defense_factor * damage);
}

void Combatant::PerformAttack(Attack attack, std::vector<Combatant*> targets)
{

    CombatantStats attacker_stats = CalculateStats();

    for (Combatant* target : targets) {
        CombatantStats target_stats = target->CalculateStats();

        bool attack_hit;

        if (attack.type == AttackType::melee) {
            attack_hit = CheckAttackHit(attacker_stats.dexterity, target_stats.dexterity, attack.accuracy, attack.effect);
        } else {
            attack_hit = CheckAttackHit(attacker_stats.perception, target_stats.perception, attack.accuracy, attack.effect);
        }

        if (!attack_hit) {
            std::cout << "Missed" << std::endl;
            continue;
        }

        bool crit_hit = CheckCritHit(attacker_stats.luck, target_stats.luck, attack.crit_chance, attack.effect);

        int damage = (crit_hit ? attack.damage : (int)(attack.damage * attack.crit_multiplier));

        int attack_damage = CalculateAttackDamage(attacker_stats.strength, target_stats.defense, damage, attack.effect);
        target->TakeDamage(attack_damage, attack.effect);
        std::cout << (crit_hit ? "Critical " : "") << "Hit for " << attack_damage << " damage\n";
    }

    cooldown_ = attack.cooldown;
}

bool Combatant::TakeDamage(int damage, AttackEffect effect)
{
    hp_ -= damage;
    
    if (hp_ <= 0) {
        hp_ = 0;
        state_ = CombatantState::dead;
        return true;
    } else {
        return false;
    }
}