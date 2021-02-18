#pragma once

#include <string>

#include "Combatant.hpp"

class EnemyCombatant : public Combatant {
  public:
	EnemyCombatant(const std::string &name, const std::string &sprite_name, int level, int max_hp, CombatantStats stats,
	               int armor);
	~EnemyCombatant();

	void AddAttack(Attack attack);
	void AddAbility(Ability ability);

	std::vector<Attack> GetAttackList();
	std::vector<Ability> GetAbilityList();
	CombatantStats CalculateStats();

  protected:
	std::vector<Attack> attacks_;
	std::vector<Ability> abilities_;

	int armor_;
};