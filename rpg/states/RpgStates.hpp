#pragma once
#include "RpgGameState.hpp"
#include "RpgInventoryState.hpp"
#include "RpgLoadGameState.hpp"
#include "RpgMenuState.hpp"
#include "RpgPlayState.hpp"

enum StateLabels : std::size_t { stateMenu, statePlay, stateInventory, stateCombat };