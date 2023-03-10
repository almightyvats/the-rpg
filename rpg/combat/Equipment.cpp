#include "Equipment.hpp"

Equipment::Equipment(const std::string &name, EquipmentType eq_type, EquipmentMaterial eq_mat, int min_level, int agi,
                     int str, int def, int dex, int perc, int luck)
{
	name_ = name;
	type_ = eq_type;
	material_ = eq_mat;
	min_level_ = min_level;
	bonus_agility_ = agi;
	bonus_strength_ = str;
	bonus_defense_ = def;
	bonus_dexterity_ = dex;
	bonus_perception_ = perc;
	bonus_luck_ = luck;
}

Equipment::~Equipment() {}

void Equipment::AddAttack(Attack attack)
{
	attacks_.push_back(attack);
}

void Equipment::AddAbility(Ability ability)
{
	abilities_.push_back(ability);
}