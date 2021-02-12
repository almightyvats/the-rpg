#include "EnemyGenerator.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

#define ENEMY_TOTAL_LEVEL_DEVIATION_FACTOR 5
#define MIN_ENEMY_LEVEL_FACTOR 0.7
#define MAX_ENEMY_OVERLEVEL 5
#define MIN_ENEMY_TEAM_SIZE 2
#define MAX_ENEMY_TEAM_SIZE 4

#define SPIRIT_STATS {(int)(1.5*lvl), 2*lvl, (int)(0.5*lvl + 1), 2*lvl, (int)(0.5*lvl + 1), lvl}
#define SPIRIT_MAX_HP 20 + 5*lvl
#define SPIRIT_ATTACK_TACKLE {"Tackle", AttackType::melee, AttackTargetType::single, 4, 0, 0.95, 0.1, 2.0, 6, AttackEffect::none}
#define SPIRIT_ATTACK_BITE {"Bite", AttackType::melee, AttackTargetType::single, 6, 0, 0.8, 0.2, 2.5, 4, AttackEffect::none}
#define SPIRIT_ABILITY_HEAL {"Forest Link", AbilityTargetType::team_multi, lvl, 0, 1.0, 10, AbilityEffect::none}

EnemyCombatant GenerateEnemySpirit(int lvl, int number)
{
    std::string name = std::string("Spirit ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_spirit", lvl, SPIRIT_MAX_HP, SPIRIT_STATS, 0);
    enemy.AddAttack(SPIRIT_ATTACK_TACKLE);
    enemy.AddAttack(SPIRIT_ATTACK_BITE);
    enemy.AddAbility(SPIRIT_ABILITY_HEAL);
    return enemy;
}

#define WILLOWISP_STATS {(int)(1.4*lvl), (int)(0.5*lvl + 1), (int)(0.7*lvl + 1), (int)(0.5*lvl + 1), 2*lvl, lvl}
#define WILLOWISP_MAX_HP 15 + 4*lvl
#define WILLOWISP_ATTACK_FIREBALL {"Fireball", AttackType::ranged, AttackTargetType::single, 3, (int)(0.5*lvl+1), 0.7, 0.0, 1.0, 7, AttackEffect::ignite}
#define WILLOWISP_ABILITY_WILDFIRE {"Wildfire", AbilityTargetType::enemy_multi, 0, 0, 1.0, 12, AbilityEffect::ignite}

EnemyCombatant GenerateEnemyWillOWisp(int lvl, int number)
{
    std::string name = std::string("Will-o'-Wisp ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_will_o_wisp", lvl, WILLOWISP_MAX_HP, WILLOWISP_STATS, 0);
    enemy.AddAttack(WILLOWISP_ATTACK_FIREBALL);
    enemy.AddAbility(WILLOWISP_ABILITY_WILDFIRE);
    return enemy;
}

#define NIGHTMARE_STATS {lvl, 0, lvl, 3*lvl, 3*lvl, lvl}
#define NIGHTMARE_MAX_HP 13
#define NIGHTMARE_ABILITY_HAUNT {"Haunt", AbilityTargetType::enemy_single, 0, 3*lvl, 1.0, 7, AbilityEffect::none}
#define NIGHTMARE_ABILITY_FEAR {"Fear", AbilityTargetType::self, 0, 0, 1.0, 3, AbilityEffect::block}

EnemyCombatant GenerateEnemyNightmare(int lvl, int number)
{
    std::string name = std::string("Nightmare ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_nightmare", lvl, NIGHTMARE_MAX_HP, NIGHTMARE_STATS, 0);
    enemy.AddAbility(NIGHTMARE_ABILITY_HAUNT);
    enemy.AddAbility(NIGHTMARE_ABILITY_FEAR);
    return enemy;
}

#define CLAYGOLEM_STATS {(int)(0.5*lvl + 1), 4*lvl, 2*lvl, (int)(0.5*lvl + 1), (int)(0.1*lvl + 1), lvl}
#define CLAYGOLEM_MAX_HP 20 + 7*lvl

EnemyCombatant GenerateEnemyClayGolem(int lvl, int number)
{
    std::string name = std::string("Clay Golem ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_clay_golem", lvl, CLAYGOLEM_MAX_HP, CLAYGOLEM_STATS, 0);
    return enemy; 
}

std::vector<EnemyCombatant> GenerateSimpleEnemies(const std::vector<Combatant*>& player_combatants)
{    
    int total_player_level = 0;
    int max_player_level = 0;
    for (auto combatant : player_combatants) {
        total_player_level += combatant->level();
        max_player_level = std::max(combatant->level(), max_player_level);
    }

    std::srand(std::time(nullptr));

    int level_deviation_factor = total_player_level / ENEMY_TOTAL_LEVEL_DEVIATION_FACTOR;
    int total_enemy_level = total_player_level + (rand() % (2 * level_deviation_factor)) - 0.5 * level_deviation_factor;

    int enemy_team_size = (rand() % (MAX_ENEMY_TEAM_SIZE - MIN_ENEMY_TEAM_SIZE + 1)) + MIN_ENEMY_TEAM_SIZE;

    std::vector<EnemyCombatant> enemy_combatants;
    
    for (int i = 0; i < enemy_team_size; i++) {
        int expected_enemy_level = total_enemy_level / (enemy_team_size - i);
        int min_enemy_level = (int)(expected_enemy_level * MIN_ENEMY_LEVEL_FACTOR);
        int max_enemy_level = 2*expected_enemy_level - min_enemy_level;

        int enemy_level = (rand() % (max_enemy_level - min_enemy_level)) + min_enemy_level;
        
        enemy_combatants.push_back(GenerateEnemyNightmare(std::min(enemy_level, max_player_level + MAX_ENEMY_OVERLEVEL), i+1));
        total_enemy_level -= enemy_level;
    }

    return enemy_combatants;
}