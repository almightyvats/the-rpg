#include "Combat.hpp"

#include <algorithm>
#include <utility>
#include <iostream>

#define MAX_INIT_CD 10

Combat::Combat(std::vector<Combatant *> player_combatants, std::vector<Combatant *> enemy_combatants)
{
	player_combatants_ = player_combatants;
	enemy_combatants_ = enemy_combatants;
}

Combatant *GetSmallestCooldownCombatant(std::vector<Combatant *> combatants)
{
	auto min_cd_combatant =
	    std::min_element(combatants.begin(), combatants.end(),
	                     [](const Combatant *a, const Combatant *b) { return a->cooldown_ < b->cooldown_; });
	return *min_cd_combatant;
}

std::vector<Combatant*> Combat::LivingPlayerCombatants()
{
	std::vector<Combatant*> living_player_combatants (player_combatants_.size());
	auto it = std::copy_if(player_combatants_.begin(), player_combatants_.end(), living_player_combatants.begin(),
	                       [](const Combatant *c) { return c->state_ != CombatantState::dead;});
    living_player_combatants.resize(std::distance(living_player_combatants.begin(), it));
	return living_player_combatants;
}

std::vector<Combatant*> Combat::LivingEnemyCombatants()
{
	std::vector<Combatant*> living_enemy_combatants (enemy_combatants_.size());
	auto it = std::copy_if(enemy_combatants_.begin(), enemy_combatants_.end(), living_enemy_combatants.begin(),
	                       [](const Combatant *c) { return c->state_ != CombatantState::dead;});
    living_enemy_combatants.resize(std::distance(living_enemy_combatants.begin(), it));
	return living_enemy_combatants;
}

Combatant* ProgressToNextAttack(std::vector<Combatant*> living_player_combatants, std::vector<Combatant*> living_enemy_combatants)
{

	Combatant *min_cd_player = GetSmallestCooldownCombatant(living_player_combatants);
	Combatant *min_cd_enemy = GetSmallestCooldownCombatant(living_enemy_combatants);

	int cooldown_progression = std::min(min_cd_player->cooldown_, min_cd_enemy->cooldown_);
	Combatant *min_cd_combatant = (min_cd_player->cooldown_ <= min_cd_enemy->cooldown_) ? min_cd_player : min_cd_enemy;

	for (Combatant *player_combatant : living_player_combatants) {
		player_combatant->cooldown_ -= cooldown_progression;
	}

	for (Combatant *enemy_combatant : living_enemy_combatants) {
		enemy_combatant->cooldown_ -= cooldown_progression;
	}

	return min_cd_combatant;
}

void Combat::PrintStatus() {
    std::cout << "Player Team:\n";

    for (Combatant* combatant : player_combatants_) {
        std::cout << "   " << combatant->name() << "  Lvl: " << combatant->level() << "  State: " << combatant->state_string() << "  HP: " << combatant->hp() << "/" << combatant->max_hp() << "  CD: " << combatant->cooldown_ << "\n";
    }

    std::cout << "Enemy Team:\n";
    for (Combatant* combatant : enemy_combatants_) {
        std::cout << "   " << combatant->name() << "  Lvl: " << combatant->level() << "  State: " << combatant->state_string() << "  HP: " << combatant->hp() << "/" << combatant->max_hp() << "  CD: " << combatant->cooldown_ << "\n";
    }
}

std::pair<int,int> GetMinMaxAgility(std::vector<Combatant *> combatants)
{
    std::pair<int,int> min_max (-1,0);

	for (Combatant* combatant : combatants) {
        if (min_max.first == -1) {
            min_max.first = combatant->agility();
            min_max.second = combatant->agility();
        } else if (combatant->agility() < min_max.first) {
            min_max.first = combatant->agility();
        } else if (combatant->agility() > min_max.second) {
            min_max.second = combatant->agility();
        }
    }

    return min_max;
}

void Combat::SetInitialCooldowns() {
    std::vector<Combatant*> all_combatants;
    all_combatants.insert(all_combatants.end(), player_combatants_.begin(), player_combatants_.end());
    all_combatants.insert(all_combatants.end(), enemy_combatants_.begin(), enemy_combatants_.end());

    std::pair<int,int> min_max = GetMinMaxAgility(all_combatants);
    int diff = min_max.second - min_max.first;

    for (Combatant* combatant : all_combatants) {
        if (diff == 0) {
            combatant->cooldown_ = 0;
        } else {
            combatant->cooldown_ = MAX_INIT_CD - (int)(MAX_INIT_CD * ((float)(combatant->agility() - min_max.first) / diff));
        }
    }
}

void Combat::Initiate()
{
    SetInitialCooldowns();

    auto living_player_combatants = LivingPlayerCombatants();
    auto living_enemy_combatants = LivingEnemyCombatants();

    while (living_player_combatants.size() > 0 && living_enemy_combatants.size() > 0) {
        PrintStatus();
        Combatant *next_attacker = ProgressToNextAttack(living_player_combatants, living_enemy_combatants);
        std::cout << "It's the turn of " << next_attacker->name() << "\n";
        next_attacker->ChooseAndPerformAction(living_player_combatants, living_enemy_combatants);
        living_player_combatants = LivingPlayerCombatants();
        living_enemy_combatants = LivingEnemyCombatants();
        std::cout << "\n";
    }

    if (living_player_combatants.size() == 0) {
        std::cout << "You lose!\n";
    } else {
        std::cout << "You win!\n";
    }   
}