#include "EnemyGenerator.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#define NUMBER_ENEMY_TYPES 9
#define NUMBER_ENEMIES_GRASS 5
#define NUMBER_ENEMIES_STONE 6

#define ENEMY_TOTAL_LEVEL_DEVIATION_FACTOR 5
#define MIN_ENEMY_LEVEL_FACTOR 0.7
#define MAX_ENEMY_OVERLEVEL 3
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


#define GOLEM_ATTACK_SMASH {"Smash", AttackType::melee, AttackTargetType::single, 10, 0, 0.8, 0.1, 1.5, 10, AttackEffect::none}
#define GOLEM_ATTACK_ROCK_THROW {"Rock Throw", AttackType::ranged, AttackTargetType::single, 10, lvl, 0.7, 0.2, 1.5, 10, AttackEffect::none}

#define CLAYGOLEM_STATS {(int)(0.5*lvl + 1), 4*lvl, 2*lvl, (int)(0.5*lvl + 1), (int)(0.5*lvl + 1), lvl}
#define CLAYGOLEM_MAX_HP 20 + 7*lvl
#define CLAYGOLEM_ATTACK_GROUND_SLAM {"Ground Slam", AttackType::melee, AttackTargetType::multi, 5, 0, 0.65, 0.0, 1.0, 12, AttackEffect::none}

EnemyCombatant GenerateEnemyClayGolem(int lvl, int number)
{
    std::string name = std::string("Clay Golem ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_clay_golem", lvl, CLAYGOLEM_MAX_HP, CLAYGOLEM_STATS, 0);
    enemy.AddAttack(GOLEM_ATTACK_SMASH);
    enemy.AddAttack(GOLEM_ATTACK_ROCK_THROW);
    enemy.AddAttack(CLAYGOLEM_ATTACK_GROUND_SLAM);
    return enemy; 
}

#define WOODGOLEM_STATS {(int)(0.7*lvl + 1), 3*lvl, 2*lvl, (int)(0.8*lvl + 1), (int)(0.8*lvl + 1), lvl}
#define WOODGOLEM_MAX_HP 5 + 8*lvl
#define WOODGOLEM_ATTACK_NATURE {"Nature", AttackType::ranged, AttackTargetType::multi, 12, lvl, 0.5, 0.0, 2.0, 12, AttackEffect::none}

EnemyCombatant GenerateEnemyWoodGolem(int lvl, int number)
{
    std::string name = std::string("Wood Golem ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_wood_golem", lvl, WOODGOLEM_MAX_HP, WOODGOLEM_STATS, 0);
    enemy.AddAttack(GOLEM_ATTACK_SMASH);
    enemy.AddAttack(GOLEM_ATTACK_ROCK_THROW);
    enemy.AddAttack(WOODGOLEM_ATTACK_NATURE);
    return enemy; 
}

#define STONEGOLEM_STATS {(int)(0.3*lvl + 1), 2*lvl, 2*lvl, (int)(0.4*lvl + 1), (int)(0.4*lvl + 1), lvl}
#define STONEGOLEM_MAX_HP 10 + 10*lvl
#define STONEGOLEM_ABILITY_HARDEN {"Harden", AbilityTargetType::self, lvl, 0, 1.0, 6, AbilityEffect::block}

EnemyCombatant GenerateEnemyStoneGolem(int lvl, int number)
{
    std::string name = std::string("Stone Golem ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_stone_golem", lvl, STONEGOLEM_MAX_HP, STONEGOLEM_STATS, 0);
    enemy.AddAttack(GOLEM_ATTACK_SMASH);
    enemy.AddAttack(GOLEM_ATTACK_ROCK_THROW);
    enemy.AddAbility(STONEGOLEM_ABILITY_HARDEN);
    return enemy; 
}

#define MINOTAUR_ATTACK_FAST_STRIKE {"Fast Strike", AttackType::melee, AttackTargetType::single, (int)(0.1*lvl + 5), 0, 0.9, 0.2, 2.0, 4, AttackEffect::none}
#define MINOTAUR_ATTACK_POWER_STRIKE {"Power Strike", AttackType::melee, AttackTargetType::single, (int)(0.25*lvl + 5), 0, 0.9, 0.2, 2.0, 9, AttackEffect::slow}
#define MINOTAUR_ABILITY_BLOCK {"Block", AbilityTargetType::self, 0, 0, 1.0, 5, AbilityEffect::block}

#define CULTIST_STATS {(int)(1.1*lvl + 1), 3*lvl, lvl, (int)(1.5*lvl + 1), (int)(1.1*lvl + 1), lvl}
#define CULTIST_MAX_HP 8*lvl
#define CULTIST_ARMOR (int)(0.5*lvl + 1)
#define CULTIST_ATTACK_WARCRY {"Warcry", AttackType::ranged, AttackTargetType::multi, 1, 3*lvl, 10.0, 0.0, 1.0, 12, AttackEffect::none}
#define CULTIST_ABILITY_CLEANSING {"Cleansing", AbilityTargetType::self, 2*lvl, 0, 1.0, 8, AbilityEffect::ignite}

EnemyCombatant GenerateEnemyCultist(int lvl, int number)
{
    std::string name = std::string("Cultist ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_minotaur_cultist", lvl, CULTIST_MAX_HP, CULTIST_STATS, CULTIST_ARMOR);
    enemy.AddAttack(MINOTAUR_ATTACK_FAST_STRIKE);
    enemy.AddAttack(MINOTAUR_ATTACK_POWER_STRIKE);
    enemy.AddAbility(MINOTAUR_ABILITY_BLOCK);
    enemy.AddAttack(CULTIST_ATTACK_WARCRY);
    enemy.AddAbility(CULTIST_ABILITY_CLEANSING);
    return enemy; 
}

#define WARRIOR_STATS {(int)(2.0*lvl + 1), 3*lvl, (int)(0.4*lvl + 1), (int)(1.5*lvl + 1), (int)(3.0*lvl + 1), lvl}
#define WARRIOR_MAX_HP 8*lvl
#define WARRIOR_ARMOR (int)(1.1*lvl)
#define WARRIOR_RAMPAGE {"Rampage", AttackType::melee, AttackTargetType::multi, (int)(0.15*lvl + 2), 0, 0.8, 0.1, 1.5, 12, AttackEffect::none}
#define WARRIOR_PURE_RAGE {"Pure Rage", AttackType::melee, AttackTargetType::single, (int)(0.5*lvl), 0, 0.7, 0.1, 2.0, 10, AttackEffect::armor_breaking}

EnemyCombatant GenerateEnemyWarrior(int lvl, int number)
{
    std::string name = std::string("Warrior ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_minotaur_warrior", lvl, WARRIOR_MAX_HP, WARRIOR_STATS, WARRIOR_ARMOR);
    enemy.AddAttack(MINOTAUR_ATTACK_FAST_STRIKE);
    enemy.AddAttack(MINOTAUR_ATTACK_POWER_STRIKE);
    enemy.AddAbility(MINOTAUR_ABILITY_BLOCK);
    enemy.AddAttack(WARRIOR_RAMPAGE);
    enemy.AddAttack(WARRIOR_PURE_RAGE);
    return enemy;
}

#define MINOCOW_STATS {(int)(1.5*lvl + 1), 3*lvl, lvl, (int)(1.5*lvl + 1), (int)(1.1*lvl + 1), lvl}
#define MINOCOW_MAX_HP 7*lvl
#define MINOCOW_ATTACK_HEADBUTT {"Headbutt", AttackType::melee, AttackTargetType::multi, 3, 0, 0.8, 0.0, 1.0, 1, AttackEffect::slow}

EnemyCombatant GenerateEnemyMinocow(int lvl, int number)
{
    std::string name = std::string("Minocow ") + std::to_string(number);
    EnemyCombatant enemy = EnemyCombatant(name, "c_minotaur_minocow", lvl, MINOCOW_MAX_HP, MINOCOW_STATS, 0);
    enemy.AddAttack(MINOTAUR_ATTACK_FAST_STRIKE);
    enemy.AddAttack(MINOTAUR_ATTACK_POWER_STRIKE);
    enemy.AddAbility(MINOTAUR_ABILITY_BLOCK);
    enemy.AddAttack(MINOCOW_ATTACK_HEADBUTT);
    return enemy;
}

static int enemy_counts[NUMBER_ENEMY_TYPES];

EnemyCombatant GenerateGrassEnemy(int lvl)
{
    int enemy_type = rand() % NUMBER_ENEMIES_GRASS;

    switch (enemy_type)
    {
    case 0:
        enemy_counts[0]++;
        return GenerateEnemySpirit(lvl, enemy_counts[0]);
    case 1:
        enemy_counts[1]++;
        return GenerateEnemyWillOWisp(lvl, enemy_counts[1]);
    case 2:
        enemy_counts[2]++;
        return GenerateEnemyNightmare(lvl, enemy_counts[2]);
    case 3:
        enemy_counts[3]++;
        return GenerateEnemyClayGolem(lvl, enemy_counts[3]);
    case 4:
        enemy_counts[4]++;
        return GenerateEnemyWoodGolem(lvl, enemy_counts[4]);
    default:
        enemy_counts[0]++;
        return GenerateEnemySpirit(lvl, enemy_counts[0]);
    }
}

EnemyCombatant GenerateStoneEnemy(int lvl)
{
    int enemy_type = rand() % NUMBER_ENEMIES_STONE;

    switch (enemy_type)
    {
    case 0:
        enemy_counts[1]++;
        return GenerateEnemyWillOWisp(lvl, enemy_counts[1]);
    case 1:
        enemy_counts[2]++;
        return GenerateEnemyNightmare(lvl, enemy_counts[2]);
    case 2:
        enemy_counts[3]++;
        return GenerateEnemyClayGolem(lvl, enemy_counts[3]);
    case 3:
        enemy_counts[5]++;
        return GenerateEnemyStoneGolem(lvl, enemy_counts[5]);
    case 4:
        enemy_counts[6]++;
        return GenerateEnemyCultist(lvl, enemy_counts[6]);
    case 5:
        enemy_counts[7]++;
        return GenerateEnemyWarrior(lvl, enemy_counts[7]);
    case 6:
        enemy_counts[8]++;
        return GenerateEnemyMinocow(lvl, enemy_counts[8]);
    default:
        enemy_counts[5]++;
        return GenerateEnemyStoneGolem(lvl, enemy_counts[5]);
    }
}

EnemyCombatant GenerateEnemy(int lvl, CombatArena arena)
{
    switch (arena)
    {
    case CombatArena::grass:
        return GenerateGrassEnemy(lvl);
    case CombatArena::stone:
        return GenerateStoneEnemy(lvl);
    default:
        return GenerateGrassEnemy(lvl);
    }
}

std::vector<EnemyCombatant> GenerateSimpleEnemies(const std::vector<Combatant*>& player_combatants, CombatArena arena)
{    
    int total_player_level = 0;
    int max_player_level = 0;
    for (auto combatant : player_combatants) {
        total_player_level += combatant->level();
        max_player_level = std::max(combatant->level(), max_player_level);
    }

    std::fill(enemy_counts, enemy_counts + NUMBER_ENEMY_TYPES, 0);

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

        enemy_combatants.push_back(GenerateEnemy(std::min(enemy_level, max_player_level + MAX_ENEMY_OVERLEVEL), arena));
        total_enemy_level -= enemy_level;
    }

    return enemy_combatants;
}