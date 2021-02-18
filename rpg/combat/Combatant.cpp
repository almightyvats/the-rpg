#include "Combatant.hpp"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

#define MIN_HIT_EVADE_FACTOR 0.5f
#define MAX_HIT_EVADE_FACTOR 1.5f
#define MIN_LUCK_FACTOR 0.8f
#define MAX_LUCK_FACTOR 2.0f

#define BLOCK_RESET_COOLDOWN 4
#define BURN_RESET_COOLDOWN 8
#define BURN_RESET_COOLDOWN_HIT_INCREASE 1
#define BURN_DAMAGE level()
#define SLOW_COOLDOWN_HIT_INCREASE 3

std::string Combatant::state_string()
{
	switch (state_) {
	case CombatantState::normal:
		return "normal";
	case CombatantState::dead:
		return "dead";
	case CombatantState::blocking:
		return "blocking";
	case CombatantState::burning:
		return "burning";
	default:
		return "unknown";
	}
}

bool CheckAttackHit(int hit_stat, int evade_stat, float accuracy, AttackEffect effect)
{
	float hit_evade_factor;

	if (evade_stat <= 0 && hit_stat > 0) {
		hit_evade_factor = MAX_HIT_EVADE_FACTOR;
	} else if (hit_stat <= 0) {
		hit_evade_factor = MIN_HIT_EVADE_FACTOR;
	} else {
		hit_evade_factor = (float)hit_stat / (float)evade_stat;
		hit_evade_factor = std::min(hit_evade_factor, MAX_HIT_EVADE_FACTOR);
		hit_evade_factor = std::max(hit_evade_factor, MIN_HIT_EVADE_FACTOR);
	}

	float roll_needed = hit_evade_factor * accuracy * 1000;
	int roll = (std::rand() % 1000);
	return roll < roll_needed;
}

bool CheckCritHit(int attacker_luck, int defender_luck, float crit_chance, AttackEffect effect)
{
	float luck_factor;

	if (defender_luck <= 0 && attacker_luck > 0) {
		luck_factor = MAX_LUCK_FACTOR;
	} else if (attacker_luck <= 0) {
		luck_factor = MIN_LUCK_FACTOR;
	} else {
		luck_factor = (float)attacker_luck / (float)defender_luck;
		luck_factor = std::min(luck_factor, MAX_LUCK_FACTOR);
		luck_factor = std::max(luck_factor, MIN_LUCK_FACTOR);
	}

	float roll_needed = luck_factor * crit_chance * 1000;
	int roll = (std::rand() % 1000);
	return roll < roll_needed;
}

int CalculateAttackDamage(int attacker_force, int defender_defense, int damage, AttackEffect effect)
{
	float force_defense_factor = (float)attacker_force / (float)defender_defense;
	return (int)(force_defense_factor * damage);
}

std::string Combatant::PerformAttack(Attack attack, std::vector<Combatant *> targets)
{
	std::ostringstream os;
	CombatantStats attacker_stats = CalculateStats();

	for (Combatant *target : targets) {

		CombatantStats target_stats = target->CalculateStats();

		bool attack_hit;

		if (attack.type == AttackType::melee) {
			attack_hit =
			    CheckAttackHit(attacker_stats.dexterity, target_stats.dexterity, attack.accuracy, attack.effect);
		} else {
			attack_hit =
			    CheckAttackHit(attacker_stats.perception, target_stats.perception, attack.accuracy, attack.effect);
		}

		if (!attack_hit) {
			os << "Missed attack on " << target->name() << "\n";
			continue;
		}

		bool crit_hit = CheckCritHit(attacker_stats.luck, target_stats.luck, attack.crit_chance, attack.effect);

		int damage = (crit_hit ? (int)(attack.damage * attack.crit_multiplier) : attack.damage);
		int attack_damage;

		int target_defense = target_stats.defense;

		if (attack.effect == AttackEffect::armor_breaking) {
			target_defense = target->defense();
		}

		if (attack.type == AttackType::melee) {
			attack_damage = CalculateAttackDamage(attacker_stats.strength, target_defense, damage, attack.effect);
		} else {
			attack_damage = CalculateAttackDamage(attack.force, target_defense, damage, attack.effect);
		}

		if (target->state_ == CombatantState::blocking) {
			target->state_ = CombatantState::normal;
			os << "Block of " << target->name() << " broken\n";
		} else {
			os << (crit_hit ? "Critical " : "") << "Hit " << target->name() << " for " << attack_damage << " damage\n";
			target->TakeDamage(attack_damage);

			if (target->state_ != CombatantState::dead) {
				if (attack.effect == AttackEffect::ignite) {
					target->TakeDamage(0);
					target->state_ = CombatantState::burning;
					target->state_reset_countdown_ = BURN_RESET_COOLDOWN;
					os << target->name() << " is now burning\n";
				} else if (attack.effect == AttackEffect::slow) {
					target->cooldown_ += SLOW_COOLDOWN_HIT_INCREASE;
					os << target->name() << " is slowed\n";
				}
			}
		}
	}

	cooldown_ = attack.cooldown;
	return os.str();
}

std::string Combatant::UseAbility(Ability ability, std::vector<Combatant *> targets)
{
	std::ostringstream os;
	std::srand(std::time(nullptr));

	for (Combatant *target : targets) {

		float roll_needed = ability.accuracy;
		int roll = (std::rand() % 1000);

		if (ability.accuracy == 1.0 || roll < roll_needed) {
			if (ability.heal != 0) {
				target->TakeDamage(-ability.heal);
			}
			if (ability.damage != 0) {
				target->TakeDamage(ability.damage);
			}

			if (ability.effect == AbilityEffect::block) {
				PerformStateReset();
				target->state_ = CombatantState::blocking;
				target->state_reset_countdown_ = BLOCK_RESET_COOLDOWN;
				os << target->name() << " started blocking\n";
			} else if (ability.effect == AbilityEffect::ignite) {
				target->TakeDamage(0);
				target->state_ = CombatantState::burning;
				target->state_reset_countdown_ = BURN_RESET_COOLDOWN;
				os << target->name() << " is now burning\n";
			}
		}
	}

	cooldown_ = ability.cooldown;
	return os.str();
}

bool Combatant::TakeDamage(int damage)
{
	hp_ -= damage;

	if (state_ == CombatantState::burning) {
		TakeBurnDamage();
		state_reset_countdown_ += BURN_RESET_COOLDOWN_HIT_INCREASE;
	}

	if (hp_ <= 0) {
		hp_ = 0;
		state_ = CombatantState::dead;
		return true;
	} else {
		return false;
	}
}

void Combatant::TakeBurnDamage()
{
	hp_ -= BURN_DAMAGE;
}

std::string Combatant::PerformStateReset()
{
	std::string display_text = "";
	switch (state_) {
	case CombatantState::blocking:
		state_ = CombatantState::normal;
		display_text = name() + " has stopped blocking";
		break;

	case CombatantState::burning:
		TakeDamage(0);
		if (state_ != CombatantState::dead) {
			state_ = CombatantState::normal;
		}
		display_text = name() + " has stopped burning";

	default:
		break;
	}
	state_reset_countdown_ = -1;
	return display_text;
}

void Combatant::ResetToIdle()
{
	hp_ = max_hp_;
	state_ = CombatantState::normal;
	cooldown_ = 0;
	state_reset_countdown_ = -1;
}