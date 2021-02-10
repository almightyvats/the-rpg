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
        PlayerCombatant(const std::string& name, const std::string& sprite_name, int level);
        PlayerCombatant(const std::string &name, const std::string& sprite_name, int level, int max_hp, int agility, int strength, int defense,
                                 int dexterity, int perception, int luck);
        ~PlayerCombatant();
        
        std::vector<Attack> GetAttackList();
        std::vector<Ability> GetAbilityList();
        CombatantStats CalculateStats();
        void ChooseAndPerformAction(const std::vector<Combatant*> player_combatants, std::vector<Combatant*> enemy_combatants);

        std::string GainExp(int exp_gain);
        std::string LevelUp(int tokens, int new_exp);
        AddEquipmentError AddEquipment(Equipment equipment);
        Equipment RemoveEquipment(int index);

        int exp() const {return exp_;}

        template <class Archive>
        void serialize(Archive &archive)
        {
            archive(CEREAL_NVP(exp_), CEREAL_NVP(base_attacks_), CEREAL_NVP(base_abilities_), CEREAL_NVP(name_),
		            CEREAL_NVP(sprite_name_), CEREAL_NVP(level_), CEREAL_NVP(max_hp_),
		            CEREAL_NVP(hp_), CEREAL_NVP(agility_), CEREAL_NVP(strength_),
		            CEREAL_NVP(defense_), CEREAL_NVP(dexterity_), CEREAL_NVP(perception_), CEREAL_NVP(luck_));
        }
    
    protected:
        std::vector<Equipment> equipment_;
        std::vector<Attack> base_attacks_;
        std::vector<Ability> base_abilities_;

        int exp_;
    
    private:
        void ChooseAttackTargetAndPerformAttack(Attack attack, std::vector<Combatant *> enemy_combatants);
        void ChooseAbilityTargetAndUseAbility(Ability ability, const std::vector<Combatant *> player_combatants, std::vector<Combatant *> enemy_combatants);
};