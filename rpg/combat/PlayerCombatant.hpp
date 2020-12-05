#pragma once

#include <string>
#include <vector>

#include "Combatant.hpp"

enum class AddEquipmentError {
    ok,
    max_equipment,
    min_level_not_met,
}

class PlayerCombatant : public Combatant {
    public:
        PlayerCombatant(const std::string& name, int level);
        ~PlayerCombatant();
        
        std::vector<Attack> GetAttackList();
        CombatantStats CalculateStats();

        void LevelUp(int tokens, int new_exp);
        AddEquipmentError AddEquipment(Equipment equipment);
        Equipment RemoveEquipment(int index);

        int exp() const {return exp_;}
    
    protected:
        std::vector<Equipment> equipment_;
        std::vector<Attack> base_attacks_;

        int exp_;
};