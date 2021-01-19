#include "PlayerCombatant.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

#define TOKENS_PER_LEVEL_UP 2
#define MAX_EQUIPMENT 5

#define ATTACK_SUCKER_PUNCH {"Sucker Punch", AttackType::melee, AttackTargetType::single, 5, 0, 0.8, 0.0, 1.0, 5, AttackEffect::none}
#define ABILITY_BLOCK {"Block", AbilityTargetType::self, 0, 0, 1.0, 5, AbilityEffect::block}

PlayerCombatant::PlayerCombatant(const std::string &name, const std::string& sprite_name, int level)
{
	name_ = name;

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

PlayerCombatant::PlayerCombatant(const std::string &name, const std::string& sprite_name, int level, int max_hp, int agility, int strength, int defense,
                                 int dexterity, int perception, int luck)
{
	name_ = name;

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

void PlayerCombatant::LevelUp(int tokens, int new_exp)
{

	std::srand(std::time(nullptr));

	level_++;
	exp_ = new_exp;

	int hp_increase = rand() % 10;
	max_hp_ += hp_increase;
	hp_ += hp_increase;

	int tokens_left = tokens;

	while (tokens_left > 0) {
		switch (rand() % 6) {
		case 0:
			agility_++;
			break;
		case 1:
			strength_++;
			break;
		case 2:
			defense_++;
			break;
		case 3:
			dexterity_++;
			break;
		case 4:
			perception_++;
			break;
		case 5:
			luck_++;
			break;
		}
		tokens_left--;
	}
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

Combatant *ChooseSingleTarget(std::vector<Combatant *> combatants)
{
	std::cout << "Available targets: \n";
	int c = 0;

	for (Combatant *combatant : combatants) {
		std::cout << c++ << " - " << combatant->name() << "\n";
	}

	int target_number;

	std::cout << "Choose target: ";
	std::cin >> target_number;

	while (target_number < 0 || target_number >= combatants.size()) {
		std::cout << "Target number our of range. Choose target: ";
		std::cin >> target_number;
	}

	return combatants.at(target_number);
}

void PlayerCombatant::ChooseAndPerformAction(const std::vector<Combatant *> player_combatants,
                                             std::vector<Combatant *> enemy_combatants)
{

	std::vector<Attack> attacks = GetAttackList();
	std::vector<Ability> abilities = GetAbilityList();

	while (true) {

		std::cout << "Available actions:\n0 - attack\n1 - use ability\nChoose action: ";
		
		int action_number;
		std::cin >> action_number;

		while (action_number < 0 || action_number > 1) {
			std::cout << "Action number our of range. Choose action: ";
			std::cin >> action_number;
		}

		if (action_number == 0) {
			std::cout << "Available attacks:\n";
			int c = 0;

			for (Attack attack : attacks) {
				std::cout << c++ << " - " << attack.name << "\n";
			}
			std::cout << c << " - BACK\n"; 

			int attack_number;

			std::cout << "Choose attack: ";
			std::cin >> attack_number;

			while (attack_number < 0 || attack_number > c) {
				std::cout << "Attack number our of range. Choose attack: ";
				std::cin >> attack_number;
			}

			if (attack_number != c) {
				ChooseAttackTargetAndPerformAttack(attacks.at(attack_number), enemy_combatants);
				break;	
			}
		} else {
			std::cout << "Available abilities:\n";
			int c = 0;

			for (Ability ability : abilities) {
				std::cout << c++ << " - " << ability.name << "\n";
			}
			std::cout << c << " - BACK\n"; 

			int ability_number;

			std::cout << "Choose ability: ";
			std::cin >> ability_number;

			while (ability_number < 0 || ability_number > c) {
				std::cout << "Ability number our of range. Choose ability: ";
				std::cin >> ability_number;
			}

			if (ability_number != c) {
				ChooseAbilityTargetAndUseAbility(abilities.at(ability_number), player_combatants, enemy_combatants);
				break;	
			}
		}	
	}
}

void PlayerCombatant::ChooseAttackTargetAndPerformAttack(Attack attack, std::vector<Combatant *> enemy_combatants)
{
	std::vector<Combatant *> targets;

	if (attack.target_type == AttackTargetType::single) {
		targets.push_back(ChooseSingleTarget(enemy_combatants));
	} else {
		targets = enemy_combatants;
	}

	PerformAttack(attack, targets);
}

void PlayerCombatant::ChooseAbilityTargetAndUseAbility(Ability ability, const std::vector<Combatant *> player_combatants, std::vector<Combatant *> enemy_combatants)
{
	std::vector<Combatant *> targets;

	switch (ability.target_type)
	{
	case AbilityTargetType::self:
		targets.push_back(this);
		break;
	case AbilityTargetType::enemy_single:
		targets.push_back(ChooseSingleTarget(enemy_combatants));
		break;
	case AbilityTargetType::enemy_multi:
		targets = enemy_combatants;
		break;
	case AbilityTargetType::team_single:
		targets.push_back(ChooseSingleTarget(player_combatants));
		break;
	case AbilityTargetType::team_multi:
		targets = player_combatants;
		break;
	case AbilityTargetType::all:
		targets.insert(targets.end(), player_combatants.begin(), player_combatants.end());
		targets.insert(targets.end(), enemy_combatants.begin(), enemy_combatants.end());
	}

	UseAbility(ability, targets);
}
