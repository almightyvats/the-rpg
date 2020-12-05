#include "PlayerCombatant.hpp"

#include <cstdlib>
#include <ctime>

#define TOKENS_PER_LEVEL_UP 2
#define MAX_EQUIPMENT 5

#define ATTACK_SUCKER_PUNCH {"Sucker Punch", melee, single, 5, 0.8, 0, 5, 2, none}

PlayerCombatant::PlayerCombatant(const std::string& name, int level) {
    name_ = name;

    level_ = 1;
    exp_ = 0;

    std::srand(std::time(nullptr));

    max_hp_ = (std::rand() % 200) + 100;
    hp_ = m_max_hp;
    state_ = CombatantState::normal;

    agility_ = (std::rand() % 10) + 1;
    strength_ = (std::rand() % 10) + 1;
    defense_ = (std::rand() % 10) + 1;
    dexterity_ = (std::rand() % 10) + 1;
    perception_ = (std::rand() % 10) + 1;
    luck_ = (std::rand() % 10) + 1;

    base_attacks_.push_back(ATTACK_SUCKER_PUNCH);

    while (level_ < level) {
        level_up(TOKENS_PER_LEVEL_UP, 0);
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
        return NULL;
    }
}

std::vector<Attack> PlayerCombatant::GetAttackList()
{
    std::vector<Attack> attacks = basic_attacks_;

    for (auto eq : equipment_) {
        attacks.insert(attacks.end(), eq.attacks_.begin(), eq.attacks_.end());
    }

    return attacks;
}

CombatantStats PlayerCombatant::CalculateStats()
{
    CombatantStats stats = {agility_, strength_, defense_, dexterity_, perception_, luck_};

    for (auto eq : equipment_) {
        stats->agility += eq.GetBonusAgility();
        stats->strength += eq.GetBonusStrength();
        stats->defense += eq.GetBonusDefense();
        stats->dexterity += eq.GetBonusDexterity();
        stats->perception += eq.GetBonusPerception();
        stats->luck += eq.GetBonusLuck();
    }

    return CombatantStats;
}