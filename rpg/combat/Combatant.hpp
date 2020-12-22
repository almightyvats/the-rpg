#pragma once

#include <string>
#include <vector>

enum class AttackType {
    melee,
    ranged,
};

enum class AttackTargetType {
    single,
    multi,
};

enum class AttackEffect {
    none,
    ignite,
};

struct Attack {
    std::string name;
    AttackType type;
    AttackTargetType target_type;
    int damage;
    int force;
    float accuracy;
    float crit_chance;
    float crit_multiplier;
    int cooldown;
    AttackEffect effect;
};

enum class AbilityTargetType {
    self,
    team_single,
    team_multi,
    enemy_single,
    enemy_multi,
    all,
};

enum class AbilityEffect {
    none,
    block,
    ignite,
};

struct Ability {
    std::string name;
    AbilityTargetType target_type;
    int heal;
    int damage;
    float accuracy;
    int cooldown;
    AbilityEffect effect;
};

struct CombatantStats {
    int agility;
    int strength;
    int defense;
    int dexterity;
    int perception;
    int luck;
};

enum class CombatantState {
    normal,
    dead,
    blocking,
    burning,
};

class Combatant {

    public:
        int cooldown_;
        CombatantState state_;
        int state_reset_countdown_;

        virtual std::vector<Attack> GetAttackList() = 0;
        virtual std::vector<Ability> GetAbilityList() = 0;
        virtual CombatantStats CalculateStats() = 0;
        virtual void ChooseAndPerformAction(const std::vector<Combatant*> player_combatants, std::vector<Combatant*> enemy_combatants) = 0;

        void PerformAttack(Attack attack, std::vector<Combatant*> targets);
        void UseAbility(Ability ability, std::vector<Combatant*> targets);
        void PerformStateReset();

        bool TakeDamage(int damage);
        void TakeBurnDamage();

        std::string name() const {return name_;}
        int level() const {return level_;}
        int max_hp() const {return max_hp_;}
        int hp() const {return hp_;}
        std::string state_string();

        int agility() const {return agility_;}
        int strength() const {return strength_;}
        int defense() const {return defense_;}
        int dexterity() const {return dexterity_;}
        int perception() const {return perception_;}
        int luck() const {return luck_;}


    protected:
        std::string name_;
        int level_;
        int max_hp_;

        int hp_;

        int agility_;      // decides who attacks first
        int strength_;     // influences melee damage
        int defense_;      // influences damage taken by melee or ranged attacks
        int dexterity_;    // influences chance to hit/evade melee attacks
        int perception_;   // influences chance to hit/evade ranged attacks
        int luck_;         // influences crit chance for melee or ranged attacks
};