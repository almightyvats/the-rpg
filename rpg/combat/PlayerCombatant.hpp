#pragma once

#include <string>
#include <vector>

#include "Combatant.hpp"
#include "Equipment.hpp"

enum class AddEquipmentError {
    ok,
    max_equipment,
    min_level_not_met,
};

class PlayerCombatant : public Combatant {
    public:
        PlayerCombatant(const std::string& name, int level);
        PlayerCombatant(const std::string &name, int level, int max_hp, int agility, int strength, int defense,
                                 int dexterity, int perception, int luck);
        ~PlayerCombatant();
        
        std::vector<Attack> GetAttackList();
        std::vector<Ability> GetAbilityList();
        CombatantStats CalculateStats();
        void ChooseAndPerformAction(const std::vector<Combatant*> player_combatants, std::vector<Combatant*> enemy_combatants);

        void LevelUp(int tokens, int new_exp);
        AddEquipmentError AddEquipment(Equipment equipment);
        Equipment RemoveEquipment(int index);

        int exp() const {return exp_;}
    
    protected:
        std::vector<Equipment> equipment_;
        std::vector<Attack> base_attacks_;
        std::vector<Ability> base_abilities_;

        int exp_;
    
    private:
        void ChooseAttackTargetAndPerformAttack(Attack attack, std::vector<Combatant *> enemy_combatants);
        void ChooseAbilityTargetAndUseAbility(Ability ability, const std::vector<Combatant *> player_combatants, std::vector<Combatant *> enemy_combatants);
};