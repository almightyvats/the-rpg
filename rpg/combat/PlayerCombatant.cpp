#include "PlayerCombatant.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

#define TOKENS_PER_LEVEL_UP 3
#define EXP_FOR_LEVEL_UP (level_ * level_)
#define MAX_EQUIPMENT 4

#define ATTACK_SUCKER_PUNCH \
	{ \
		"Sucker Punch", AttackType::melee, AttackTargetType::single, 5, 0, 0.95, 0.0, 1.0, 5, AttackEffect::none \
	}
#define ABILITY_BLOCK \
	{ \
		"Block", AbilityTargetType::self, 0, 0, 1.0, 5, AbilityEffect::block \
	}

PlayerCombatant::PlayerCombatant(const std::string &name, const std::string &sprite_name, int level)
{
	name_ = name;
	sprite_name_ = sprite_name;

	level_ = 1;
	exp_ = 0;

	std::srand(std::time(nullptr));

	max_hp_ = (std::rand() % 200) + 100;
	hp_ = max_hp_;
	state_ = CombatantState::normal;
	cooldown_ = 0;
	state_reset_countdown_ = -1;

	agility_ = (std::rand() % 10) + 1;
	strength_ = (std::rand() % 10) + 1;
	defense_ = (std::rand() % 10) + 1;
	dexterity_ = (std::rand() % 10) + 1;
	perception_ = (std::rand() % 10) + 1;
	luck_ = (std::rand() % 10) + 1;

	Attack base_attack1 = ATTACK_SUCKER_PUNCH;
	base_attacks_.push_back(base_attack1);

	Ability base_ability1 = ABILITY_BLOCK;
	base_abilities_.push_back(base_ability1);

	while (level_ < level) {
		LevelUp(TOKENS_PER_LEVEL_UP, 0);
	}
}

PlayerCombatant::PlayerCombatant(const std::string &name, const std::string &sprite_name, int level, int max_hp,
                                 int agility, int strength, int defense, int dexterity, int perception, int luck)
{
	name_ = name;
	sprite_name_ = sprite_name;

	level_ = level;
	exp_ = 0;

	max_hp_ = max_hp;
	hp_ = max_hp_;
	state_ = CombatantState::normal;
	cooldown_ = 0;
	state_reset_countdown_ = 0;

	agility_ = agility;
	strength_ = strength;
	defense_ = defense;
	dexterity_ = dexterity;
	perception_ = perception;
	luck_ = luck;

	Attack base_attack1 = ATTACK_SUCKER_PUNCH;
	base_attacks_.push_back(base_attack1);

	Ability base_ability1 = ABILITY_BLOCK;
	base_abilities_.push_back(base_ability1);
}

PlayerCombatant::~PlayerCombatant()
{
	// for image in frontend
}

std::string PlayerCombatant::GainExp(int exp_gain)
{
	std::ostringstream os;
	exp_ += exp_gain;
	int exp_for_level_up = EXP_FOR_LEVEL_UP;
	os << name() << " gained " << exp_gain << " Exp.";
	while (exp_ > EXP_FOR_LEVEL_UP) {
		os << "\n";
		os << LevelUp(TOKENS_PER_LEVEL_UP, exp_ - EXP_FOR_LEVEL_UP);
	}
	os << "   . . .  " << exp_ << "/" << EXP_FOR_LEVEL_UP << " Exp. until next Level Up\n";
	return os.str();
}

std::string PlayerCombatant::LevelUp(int tokens, int new_exp)
{
	std::ostringstream os;

	level_++;
	exp_ = new_exp;

	int hp_increase = rand() % 10;
	max_hp_ += hp_increase;
	hp_ += hp_increase;

	os << "Level Up! " << name() << " is now Lvl. " << level_ << " (+" << hp_increase << " HP";

	int tokens_left = tokens;

	while (tokens_left > 0) {
		switch (rand() % 6) {
		case 0:
			agility_ += 2;
			os << ",+2 AGI";
			break;
		case 1:
			strength_ += 2;
			os << ",+2 STR";
			break;
		case 2:
			defense_ += 2;
			os << ",+2 DEF";
			break;
		case 3:
			dexterity_ += 2;
			os << ",+2 DEX";
			break;
		case 4:
			perception_ += 2;
			os << ",+2 PER";
			break;
		case 5:
			luck_ += 2;
			os << ",+2 LCK";
			break;
		}
		tokens_left--;
	}
	os << ")";
	return os.str();
}

AddEquipmentError PlayerCombatant::AddEquipment(Equipment equipment)
{
	if (equipment_.size() >= MAX_EQUIPMENT) {
		return AddEquipmentError::max_equipment;
	} else if (equipment.min_level() > level_) {
		return AddEquipmentError::min_level_not_met;
	} else {
		equipment_.push_back(equipment);
		return AddEquipmentError::ok;
	}
}

Equipment PlayerCombatant::RemoveEquipment(int index)
{
	if (index < equipment_.size()) {
		Equipment equipment = equipment_.at(index);
		equipment_.erase(equipment_.begin() + index);
		return equipment;
	} else {
		return equipment_.at(index);
	}
}

void PlayerCombatant::ClearEquipment()
{
	equipment_.clear();
}

std::vector<Attack> PlayerCombatant::GetAttackList()
{
	std::vector<Attack> attacks = base_attacks_;

	for (auto eq : equipment_) {
		std::vector<Attack> equipment_attacks = eq.attacks();
		attacks.insert(attacks.end(), equipment_attacks.begin(), equipment_attacks.end());
	}

	return attacks;
}

std::vector<Ability> PlayerCombatant::GetAbilityList()
{
	std::vector<Ability> abilities = base_abilities_;

	for (auto eq : equipment_) {
		std::vector<Ability> equipment_abilities = eq.abilities();
		abilities.insert(abilities.end(), equipment_abilities.begin(), equipment_abilities.end());
	}

	return abilities;
}

CombatantStats PlayerCombatant::CalculateStats()
{
	CombatantStats stats = {agility_, strength_, defense_, dexterity_, perception_, luck_};

	for (auto eq : equipment_) {
		stats.agility += eq.bonus_agility();
		stats.strength += eq.bonus_strength();
		stats.defense += eq.bonus_defense();
		stats.dexterity += eq.bonus_dexterity();
		stats.perception += eq.bonus_perception();
		stats.luck += eq.bonus_luck();
	}

	return stats;
}