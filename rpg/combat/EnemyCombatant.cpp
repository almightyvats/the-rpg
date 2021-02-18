#include "EnemyCombatant.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

EnemyCombatant::EnemyCombatant(const std::string &name, const std::string &sprite_name, int level, int max_hp,
                               CombatantStats stats, int armor)
{
	name_ = name;
	level_ = level;
	sprite_name_ = sprite_name;

	max_hp_ = max_hp;
	hp_ = max_hp_;
	state_ = CombatantState::normal;
	cooldown_ = 0;
	state_reset_countdown_ = -1;

	agility_ = stats.agility;
	strength_ = stats.strength;
	defense_ = stats.defense;
	dexterity_ = stats.dexterity;
	perception_ = stats.perception;
	luck_ = stats.luck;

	armor_ = armor;
}

EnemyCombatant::~EnemyCombatant() {}

void EnemyCombatant::AddAttack(Attack attack)
{
	attacks_.push_back(attack);
}

void EnemyCombatant::AddAbility(Ability ability)
{
	abilities_.push_back(ability);
}

std::vector<Attack> EnemyCombatant::GetAttackList()
{
	return attacks_;
}

std::vector<Ability> EnemyCombatant::GetAbilityList()
{
	return abilities_;
}

CombatantStats EnemyCombatant::CalculateStats()
{
	return {agility_, strength_, defense_ + armor_, dexterity_, perception_, luck_};
}