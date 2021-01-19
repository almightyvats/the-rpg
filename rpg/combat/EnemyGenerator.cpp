#include "EnemyGenerator.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

#define ENEMY_TOTAL_LEVEL_DEVIATION_FACTOR 5
#define MIN_ENEMY_LEVEL_FACTOR 0.7
#define MIN_ENEMY_TEAM_SIZE 1
#define MAX_ENEMY_TEAM_SIZE 4

#define WRAITH_STATS {(int)(1.5*lvl), 2*lvl, (int)(0.5*lvl + 1), 2*lvl, (int)(0.5*lvl + 1), lvl}
#define WRAITH_MAX_HP 20 + 5*lvl
#define WRAITH_ATTACK_TACKLE {"Tackle", AttackType::melee, AttackTargetType::single, 4, 0, 0.95, 0.1, 2.0, 6, AttackEffect::none}
#define WRAITH_ATTACK_BITE {"Bite", AttackType::melee, AttackTargetType::single, 6, 0, 0.8, 0.2, 2.5, 4, AttackEffect::none}

EnemyCombatant GenerateEnemyWraith(int lvl, int number)
{
    std::string name = std::string("Wraith ") + std::to_string(number);
    EnemyCombatant wraith = EnemyCombatant(name, "c_wraith1", lvl, WRAITH_MAX_HP, WRAITH_STATS, 0);
    wraith.AddAttack(WRAITH_ATTACK_TACKLE);
    wraith.AddAttack(WRAITH_ATTACK_BITE);
    return wraith;
}

std::vector<EnemyCombatant> GenerateSimpleEnemies(const std::vector<Combatant*>& player_combatants)
{    
    int total_player_level = 0;
    for (auto combatant : player_combatants) {
        total_player_level += combatant->level();
    }

    std::srand(std::time(nullptr));

    int level_deviation_factor = total_player_level / ENEMY_TOTAL_LEVEL_DEVIATION_FACTOR;
    int total_enemy_level = total_player_level + (rand() % (2 * level_deviation_factor)) - 0.5 * level_deviation_factor;

    int enemy_team_size = (rand() % (MAX_ENEMY_TEAM_SIZE - MIN_ENEMY_TEAM_SIZE)) + MIN_ENEMY_TEAM_SIZE;

    std::vector<EnemyCombatant> enemy_combatants;
    
    for (int i = 0; i < enemy_team_size; i++) {
        int expected_enemy_level = total_enemy_level / (enemy_team_size - i);
        int min_enemy_level = (int)(expected_enemy_level * MIN_ENEMY_LEVEL_FACTOR);
        int max_enemy_level = 2*expected_enemy_level - min_enemy_level;

        int enemy_level = (rand() % (max_enemy_level - min_enemy_level)) + min_enemy_level;
        
        enemy_combatants.push_back(GenerateEnemyWraith(enemy_level, i+1));
        total_enemy_level -= enemy_level;
    }

    return enemy_combatants;
}