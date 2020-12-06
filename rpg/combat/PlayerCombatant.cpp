#include "PlayerCombatant.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

#define TOKENS_PER_LEVEL_UP 2
#define MAX_EQUIPMENT 5

#define ATTACK_SUCKER_PUNCH {"Sucker Punch", AttackType::melee, AttackTargetType::single, 5, 0.8, 0.0, 1.0, 5, AttackEffect::none}

PlayerCombatant::PlayerCombatant(const std::string& name, int level) {
    name_ = name;

    level_ = 1;
    exp_ = 0;

    std::srand(std::time(nullptr));

    max_hp_ = (std::rand() % 200) + 100;
    hp_ = max_hp_;
    state_ = CombatantState::normal;

    agility_ = (std::rand() % 10) + 1;
    strength_ = (std::rand() % 10) + 1;
    defense_ = (std::rand() % 10) + 1;
    dexterity_ = (std::rand() % 10) + 1;
    perception_ = (std::rand() % 10) + 1;
    luck_ = (std::rand() % 10) + 1;

    Attack base_attack1 = ATTACK_SUCKER_PUNCH;
    base_attacks_.push_back(base_attack1);

    while (level_ < level) {
        LevelUp(TOKENS_PER_LEVEL_UP, 0);
    }
}

PlayerCombatant::~PlayerCombatant() {
    // for image in frontend
}

void PlayerCombatant::LevelUp(int tokens, int new_exp) {
    
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
    } else if (equipment.min_level() > level_){
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