#pragma once

#include <cereal/types/vector.hpp>
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
	armor_breaking,
	slow,
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

	template <class Archive>
	void serialize(Archive &archive)
	{
		archive(CEREAL_NVP(name), CEREAL_NVP(type), CEREAL_NVP(target_type), CEREAL_NVP(damage), CEREAL_NVP(force),
		        CEREAL_NVP(accuracy), CEREAL_NVP(crit_chance), CEREAL_NVP(crit_multiplier), CEREAL_NVP(cooldown),
		        CEREAL_NVP(effect)); // serialize things by passing them to the archive
	}
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

	template <class Archive>
	void serialize(Archive &archive)
	{
		archive(CEREAL_NVP(name), CEREAL_NVP(heal), CEREAL_NVP(target_type), CEREAL_NVP(damage), CEREAL_NVP(accuracy),
		        CEREAL_NVP(cooldown),
		        CEREAL_NVP(effect)); // serialize things by passing them to the archive
	}
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
	virtual void ChooseAndPerformAction(const std::vector<Combatant *> player_combatants,
	                                    std::vector<Combatant *> enemy_combatants) = 0;

	std::string PerformAttack(Attack attack, std::vector<Combatant *> targets);
	std::string UseAbility(Ability ability, std::vector<Combatant *> targets);
	std::string PerformStateReset();

	bool TakeDamage(int damage);
	void TakeBurnDamage();
	void ResetToIdle();

	std::string name() const { return name_; }
	std::string sprite_name() const { return sprite_name_; }
	int level() const { return level_; }
	int max_hp() const { return max_hp_; }
	int hp() const { return hp_; }
	std::string state_string();

	int agility() const { return agility_; }
	int strength() const { return strength_; }
	int defense() const { return defense_; }
	int dexterity() const { return dexterity_; }
	int perception() const { return perception_; }
	int luck() const { return luck_; }

  protected:
	std::string name_;
	std::string sprite_name_;
	int level_;
	int max_hp_;

	int hp_;

	int agility_;    // decides who attacks first
	int strength_;   // influences melee damage
	int defense_;    // influences damage taken by melee or ranged attacks
	int dexterity_;  // influences chance to hit/evade melee attacks
	int perception_; // influences chance to hit/evade ranged attacks
	int luck_;       // influences crit chance for melee or ranged attacks
};