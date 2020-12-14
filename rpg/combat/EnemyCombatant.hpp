#pragma once

#include <string>

#include "Combatant.hpp"

class EnemyCombatant : public Combatant {
    public:
        EnemyCombatant(const std::string& name, int level, int max_hp, CombatantStats stats, int armor);
        ~EnemyCombatant();

        void AddAttack(Attack attack);
        
        std::vector<Attack> GetAttackList();
        CombatantStats CalculateStats();
        void ChooseAndPerformAction(const std::vector<Combatant*> player_combatants, std::vector<Combatant*> enemy_combatants);
    
    protected:
        std::vector<Attack> attacks_;

        int armor_;
};