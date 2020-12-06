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
};

struct Attack {
    std::string name;
    AttackType type;
    AttackTargetType target_type;
    int damage;
    float accuracy;
    float crit_chance;
    float crit_multiplier;
    int cooldown;
    AttackEffect effect;
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
};

class Combatant {

    public:
        int cooldown_;
        CombatantState state_;

        virtual std::vector<Attack> GetAttackList() = 0;
        virtual CombatantStats CalculateStats() = 0;

        void PerformAttack(Attack attack, std::vector<Combatant*> targets);

        int level() const {return level_;}
        int max_hp() const {return max_hp_;}
        int hp() const {return hp_;}

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