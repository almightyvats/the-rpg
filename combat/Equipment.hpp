#pragma once

#include <string>
#include <vector>

struct Attack {
    std::string name;
    int m_type;
    int m_damage;
    int m_accuracy;
    int m_crit_chance;
    int m_crit_multiplier;
};

class Equipment {
    public:
        Equipment();
        ~Equipment();

    protected:
        std::string m_name;
        int m_class;

        std::vector<Attack> attacks;

        int m_bonus_agility;
        int m_bonus_strength;
        int m_bonus_defense;
        int m_bonus_dexterity;
        int m_bonus_perception;
        int m_bonus_luck;  
};