#include "EnemyCombatant.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

EnemyCombatant::EnemyCombatant(const std::string& name, int level, int max_hp, CombatantStats stats, int armor)
{
    name_ = name;
    level_ = level;
    
    max_hp_ = max_hp;
    hp_ = max_hp_;
    state_ = CombatantState::normal;
    cooldown_ = 0;

    agility_ = stats.agility;
    strength_ = stats.strength;
    defense_ = stats.defense;
    dexterity_ = stats.dexterity;
    perception_ = stats.perception;
    luck_ = stats.luck;

    armor_ = armor;
}

EnemyCombatant::~EnemyCombatant()
{

}

void EnemyCombatant::AddAttack(Attack attack)
{
    attacks_.push_back(attack);
}

std::vector<Attack> EnemyCombatant::GetAttackList()
{
    return attacks_;
}

CombatantStats EnemyCombatant::CalculateStats()
{
    return {agility_, strength_, defense_ + armor_, dexterity_, perception_, luck_};
}

void EnemyCombatant::ChooseAndPerformAction(const std::vector<Combatant*> player_combatants, std::vector<Combatant*> enemy_combatants)
{
    
    std::vector<Attack> attacks = GetAttackList();
    
    std::srand(std::time(nullptr));
    Attack chosen_attack = attacks.at(std::rand() % attacks.size());

    if (chosen_attack.target_type == AttackTargetType::multi) {
        std::cout << name_ << " uses " << chosen_attack.name << "\n";
        PerformAttack(chosen_attack, player_combatants);
    } else {
        Combatant* target = player_combatants.at(std::rand() % player_combatants.size());
        std::vector<Combatant*> targets;
        targets.push_back(target);
        std::cout << name_ << " uses " << chosen_attack.name << " against " << target->name() << "\n";
        PerformAttack(chosen_attack, targets);
    }
}