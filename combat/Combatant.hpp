#pragma once

#include <string>

class Combatant {
    public:
        Combatant(const std::string& name, int level);
        ~Combatant();
    
    protected:
        std::string m_name;

        std::vector<Equipment> equipment;

        int m_level;
        int m_exp;

        int m_max_hp;
        int m_hp;
        int m_state;

        int m_agility;      // decides who attacks first
        int m_strength;     // influences melee damage
        int m_defense;      // influences damage taken by melee or ranged attacks
        int m_dexterity;    // influences chance to hit/evade melee attacks
        int m_perception;   // influences chance to hit/evade ranged attacks
        int m_luck;         // influences crit chance for melee or ranged attacks
};