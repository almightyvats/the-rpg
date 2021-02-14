#pragma once

#include <vector>
#include <string>

#include "Combatant.hpp"
#include "PlayerCombatant.hpp"
#include "EnemyCombatant.hpp"

enum class CombatArena {
    grass,
    stone,
};

enum class CombatState {
    start,
    turn_start_display,
    action_selection,
    attack_target_selection,
    ability_target_selection,
    action_display,
    state_reset_display,
    winning_screen,
    exp_gain_display,
    loot_display,
    losing_screen,
    escape_screen,
};

class Combat {
    public:
        Combat(std::vector<Combatant*> player_combatants, std::vector<Combatant*> enemy_combatants);
        Combat();

        std::vector<Combatant*> player_combatants_;
        std::vector<Combatant*> enemy_combatants_;

        void PrintStatus();
        void Initiate();

        void Progress(Attack* attack, Ability* ability, Combatant* target);
        void Flee();

        CombatState state() const {return state_;}
        std::string display_text() const {return display_text_;}
        bool player_turn() const {return player_turn_;}
        Combatant* active_combatant() const {return active_combatant_;}
        Attack* active_turn_chosen_attack() {return active_turn_chosen_attack_;}
        Ability* active_turn_chosen_ability() {return active_turn_chosen_ability_;}
        std::vector<Attack> active_turn_attacks() const {return active_turn_attacks_;}
        std::vector<Ability> active_turn_abilities() const {return active_turn_abilities_;}
        std::vector<Combatant*> active_turn_targets() {return active_turn_targets_;}
        Equipment loot() const {return loot_;}

    protected:
        CombatState state_;
        std::string display_text_ = "";
        
        bool player_turn_;
        Combatant* active_combatant_;
        Attack* active_turn_chosen_attack_;
        Ability* active_turn_chosen_ability_;
        std::vector<Attack> active_turn_attacks_;
        std::vector<Ability> active_turn_abilities_;
        std::vector<Combatant*> active_turn_targets_;
        Equipment loot_;

    private:
        int min_pc_lvl_;
        int max_pc_lvl_;
        int sum_ec_lvl_;

        std::vector<Combatant*> LivingPlayerCombatants();
        std::vector<Combatant*> LivingEnemyCombatants();
        std::vector<Combatant*> AllLivingCombatants();
        void SetInitialCooldowns();
        void ProgressToNextAction();
        void StartTurn();
        void PerformEnemyTurn();
        void SetActionAndProgress(Attack* attack, Ability* ability);
        void DistributeCombatExp();

};