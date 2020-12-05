#include "EnemyCombatant.hpp"

EnemyCombatant::EnemyCombatant(const std::string& name, int level, int max_hp, CombatantStats stats, int armor) {
    name_ = name;
    level_ = level;
    
    max_hp_ = max_hp;
    hp_ = m_max_hp;
    state_ = CombatantState::normal;

    agility_ = stats.agility;
    strength_ = stats.strength;
    defense_ = stats.defense;
    dexterity_ = stats.dexterity;
    perception_ = stats.perception;
    luck_ = stats.luck;

    armor_ = armor;
}

EnemyCombatant::~EnemyCombatant() {

}

EnemyCombatant::GetAttackList() {
    return attacks_;
}

EnemyCombatant::CalculateStats() {
    return {agility_, strength_, defense_ + armor_, dexterity_, perception_, luck_};
}