#include "Combat.hpp"
#include "Combatant.hpp"
#include "PlayerCombatant.hpp"
#include "EnemyCombatant.hpp"

#include <vector>

std::vector<EnemyCombatant> GenerateSimpleEnemies(const std::vector<Combatant*>& player_combatants, CombatArena arena);