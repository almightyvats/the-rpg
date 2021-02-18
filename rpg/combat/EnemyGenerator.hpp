#pragma once

#include "Combat.hpp"
#include "Combatant.hpp"
#include "EnemyCombatant.hpp"
#include "PlayerCombatant.hpp"

#include <vector>

std::vector<EnemyCombatant> GenerateSimpleEnemies(const std::vector<Combatant *> &player_combatants, CombatArena arena);