#pragma once

#include <string>
#include <vector>

#include "Combatant.hpp"

enum class EquipmentType {
    sword,
    axe,
    bow,
    shield,
    heal_item,
};

enum class EquipmentMaterial {
    wood,
    metal,
    gold,
    fire,
    ice,
    special,
};

class Equipment {
    public:
        Equipment(const std::string &name, EquipmentType eq_type, EquipmentMaterial eq_mat, int min_level, int agi, int str, int def, int dex,
                     int perc, int luck);
        Equipment() = default;
        ~Equipment();

        void AddAttack(Attack attack);
        void AddAbility(Ability ability);

        std::string name() const {return name_;}
        EquipmentType type() const {return type_;}
        EquipmentMaterial material() const {return material_;}
        int min_level() const {return min_level_;}

        std::vector<Attack> attacks() const {return attacks_;}
        std::vector<Ability> abilities() const {return abilities_;}
        int bonus_agility() const {return bonus_agility_;}
        int bonus_strength() const {return bonus_strength_;}
        int bonus_defense() const {return bonus_defense_;}
        int bonus_dexterity() const {return bonus_dexterity_;}
        int bonus_perception() const {return bonus_perception_;}
        int bonus_luck() const {return bonus_luck_;}

    protected:
        std::string name_;
        EquipmentType type_;
        EquipmentMaterial material_;

        int min_level_;

        std::vector<Attack> attacks_;
        std::vector<Ability> abilities_;

        int bonus_agility_;
        int bonus_strength_;
        int bonus_defense_;
        int bonus_dexterity_;
        int bonus_perception_;
        int bonus_luck_;  
};