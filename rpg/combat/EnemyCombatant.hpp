#pragma once

#include <string>

#include "Combatant.hpp"

class EnemyCombatant : public Combatant {
    public:
        EnemyCombatant(const std::string& name, int level, int max_hp, CombatantStats stats, int armor);
        ~EnemyCombatant();
        
        std::vector<Attack> GetAttackList();
        CombatantStats CalculateStats();
    
    protected:
        std::vector<Attack> attacks;

        int armor_;
}