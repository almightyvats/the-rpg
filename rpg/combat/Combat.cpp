#include "Combat.hpp"
#include "LootGenerator.hpp"

#include <algorithm>
#include <utility>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define MAX_INIT_CD 10
#define VIS_COMBAT_PLAYER_C_LIMIT 3
#define VIS_COMBAT_ENEMY_C_LIMIT 4

Combat::Combat(std::vector<Combatant *> player_combatants, std::vector<Combatant *> enemy_combatants)
{
	player_combatants_ = player_combatants;
	enemy_combatants_ = enemy_combatants;
    state_ = CombatState::start;
    player_turn_ = false;
    min_pc_lvl_ = 99999;
    max_pc_lvl_ = 0;
    sum_ec_lvl_ = 0;

    for (auto player_combatant : player_combatants_) {
        min_pc_lvl_ = std::min(min_pc_lvl_, player_combatant->level());
        max_pc_lvl_ = std::max(min_pc_lvl_, player_combatant->level());
    }

    for (auto enemy_combatant : enemy_combatants_) {
        sum_ec_lvl_ += enemy_combatant->level();
    }
}

Combat::Combat()
{
    //auto-init empty vectors
    state_ = CombatState::start;
    player_turn_ = false;
}

Combatant *GetSmallestCooldownCombatant(std::vector<Combatant *> combatants)
{
	auto min_cd_combatant =
	    std::min_element(combatants.begin(), combatants.end(),
	                     [](const Combatant *a, const Combatant *b) { return std::min(a->cooldown_, (a->state_reset_countdown_ < 0 ? a->cooldown_ +1 : a->state_reset_countdown_)) < std::min(b->cooldown_, (b->state_reset_countdown_ < 0 ? b->cooldown_ +1 : b->state_reset_countdown_)); });
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

std::vector<Combatant*> Combat::AllLivingCombatants()
{
    std::vector<Combatant*> living_combatants;
    std::vector<Combatant*> player_combatants = LivingPlayerCombatants();
    std::vector<Combatant*> enemy_combatants = LivingEnemyCombatants();
    living_combatants.insert(living_combatants.end(), player_combatants.begin(), player_combatants.end());
    living_combatants.insert(living_combatants.end(), enemy_combatants.begin(), enemy_combatants.end());
    return living_combatants;
}

Combatant* ProgressToNextAction_old(std::vector<Combatant*> living_player_combatants, std::vector<Combatant*> living_enemy_combatants)
{

	Combatant *min_cd_player = GetSmallestCooldownCombatant(living_player_combatants);
    int cd_player = std::min(min_cd_player->cooldown_, (min_cd_player->state_reset_countdown_ < 0 ? min_cd_player->cooldown_ +1 : min_cd_player->state_reset_countdown_));

	Combatant *min_cd_enemy = GetSmallestCooldownCombatant(living_enemy_combatants);
    int cd_enemy = std::min(min_cd_enemy->cooldown_, (min_cd_enemy->state_reset_countdown_ < 0 ? min_cd_enemy->cooldown_ +1 : min_cd_enemy->state_reset_countdown_));

	int cooldown_progression = std::min(cd_player, cd_enemy);

    Combatant *min_cd_combatant;
    if (cd_player == cd_enemy && cd_player == min_cd_player->cooldown_ && cd_enemy == min_cd_enemy->state_reset_countdown_) {
        min_cd_combatant = min_cd_enemy;
    } else {
	    min_cd_combatant = (cd_player <= cd_enemy) ? min_cd_player : min_cd_enemy;
    }

	for (Combatant *player_combatant : living_player_combatants) {
		player_combatant->cooldown_ -= cooldown_progression;
        if (player_combatant->state_reset_countdown_ > 0) {
            player_combatant->state_reset_countdown_ -= cooldown_progression;
        }
	}

	for (Combatant *enemy_combatant : living_enemy_combatants) {
		enemy_combatant->cooldown_ -= cooldown_progression;
        if (enemy_combatant->state_reset_countdown_ > 0) {
            enemy_combatant->state_reset_countdown_ -= cooldown_progression;
        }
	}

	return min_cd_combatant;
}

void Combat::PrintStatus() {
    std::cout << "Player Team:\n";

    for (Combatant* combatant : player_combatants_) {
        std::cout << "   " << combatant->name() << "  Lvl: " << combatant->level() << "  State: " << combatant->state_string() << " " << (combatant->state_reset_countdown_ < 0 ? 0 : combatant->state_reset_countdown_) << " HP: " << combatant->hp() << "/" << combatant->max_hp() << "  CD: " << combatant->cooldown_ << "\n";
    }

    std::cout << "Enemy Team:\n";
    for (Combatant* combatant : enemy_combatants_) {
        std::cout << "   " << combatant->name() << "  Lvl: " << combatant->level() << "  State: " << combatant->state_string() << " " << (combatant->state_reset_countdown_ < 0 ? 0 : combatant->state_reset_countdown_) << "  HP: " << combatant->hp() << "/" << combatant->max_hp() << "  CD: " << combatant->cooldown_ << "\n";
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

void Combat::SetInitialCooldowns()
{
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
        if (combatant->state_ == CombatantState::normal) {
            combatant->state_reset_countdown_ = -1;
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
        Combatant *next_attacker = ProgressToNextAction_old(living_player_combatants, living_enemy_combatants);
        if (next_attacker->state_reset_countdown_ < 0 || next_attacker->state_reset_countdown_ > next_attacker->cooldown_) {
            std::cout << "It's the turn of " << next_attacker->name() << "\n";
            next_attacker->ChooseAndPerformAction(living_player_combatants, living_enemy_combatants);
        } else {
            next_attacker->PerformStateReset();
        }
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

void Combat::ProgressToNextAction()
{
    active_turn_chosen_attack_ = NULL;
    active_turn_chosen_ability_ = NULL;
    active_combatant_ = NULL;

    std::vector<Combatant*> living_player_combatants = LivingPlayerCombatants();
    std::vector<Combatant*> living_enemy_combatants = LivingEnemyCombatants();

    if (living_player_combatants.size() == 0) {
        state_ = CombatState::losing_screen;
        return;
    } else if (living_enemy_combatants.size() == 0) {
        state_ = CombatState::winning_screen;
        return;
    }

	Combatant *min_cd_player = GetSmallestCooldownCombatant(living_player_combatants);
    int cd_player = std::min(min_cd_player->cooldown_, (min_cd_player->state_reset_countdown_ < 0 ? min_cd_player->cooldown_ +1 : min_cd_player->state_reset_countdown_));

	Combatant *min_cd_enemy = GetSmallestCooldownCombatant(living_enemy_combatants);
    int cd_enemy = std::min(min_cd_enemy->cooldown_, (min_cd_enemy->state_reset_countdown_ < 0 ? min_cd_enemy->cooldown_ +1 : min_cd_enemy->state_reset_countdown_));

	int cooldown_progression = std::min(cd_player, cd_enemy);

    Combatant *min_cd_combatant;
    if (cd_player == cd_enemy && cd_player == min_cd_player->cooldown_ && cd_enemy == min_cd_enemy->state_reset_countdown_) {
        min_cd_combatant = min_cd_enemy;
    } else {
	    min_cd_combatant = (cd_player <= cd_enemy) ? min_cd_player : min_cd_enemy;
    }

    this->player_turn_ = min_cd_combatant == min_cd_player;

	for (Combatant *player_combatant : living_player_combatants) {
		player_combatant->cooldown_ -= cooldown_progression;
        if (player_combatant->state_reset_countdown_ > 0) {
            player_combatant->state_reset_countdown_ -= cooldown_progression;
        }
	}

	for (Combatant *enemy_combatant : living_enemy_combatants) {
		enemy_combatant->cooldown_ -= cooldown_progression;
        if (enemy_combatant->state_reset_countdown_ > 0) {
            enemy_combatant->state_reset_countdown_ -= cooldown_progression;
        }
	}

    this->active_combatant_ = min_cd_combatant;

    if (min_cd_combatant->state_reset_countdown_ < 0 || min_cd_combatant->state_reset_countdown_ > min_cd_combatant->cooldown_) {     
        this->active_turn_attacks_ = min_cd_combatant->GetAttackList();
        this->active_turn_abilities_ = min_cd_combatant->GetAbilityList();
        this->state_ = CombatState::turn_start_display;
    } else {
        this->display_text_ = active_combatant_->PerformStateReset();
        this->state_ = CombatState::state_reset_display;                     
    }
}

void Combat::StartTurn()
{
    display_text_ = "";
    if (player_turn_) {
        this->state_ = CombatState::action_selection;
    } else {
        PerformEnemyTurn();
    }
}

void Combat::PerformEnemyTurn()
{
    std::vector<Combatant*> living_player_combatants = LivingPlayerCombatants();
    std::vector<Combatant*> living_enemy_combatants = LivingEnemyCombatants();

    int attacks_size = active_turn_attacks_.size();
    int abilities_size = active_turn_abilities_.size();
    std::srand(std::time(nullptr));

    int action_number = std::rand() % (attacks_size + abilities_size);
    if (action_number < attacks_size) {
        active_turn_chosen_attack_ = &(active_turn_attacks_.at(action_number));
        if (active_turn_chosen_attack_->target_type == AttackTargetType::single) {
            display_text_ = active_combatant_->PerformAttack(*active_turn_chosen_attack_, {living_player_combatants.at(std::rand() % living_player_combatants.size())});
        } else {
            display_text_ = active_combatant_->PerformAttack(*active_turn_chosen_attack_, living_player_combatants);
        }
    } else {
        active_turn_chosen_ability_ = &(active_turn_abilities_.at(action_number - attacks_size));

        std::vector<Combatant*> targets;

        switch (active_turn_chosen_ability_->target_type)
        {
        case AbilityTargetType::self: targets = {active_combatant_}; break;
        case AbilityTargetType::enemy_single: targets = {living_player_combatants.at(std::rand() % living_player_combatants.size())};
                                                break;
        case AbilityTargetType::enemy_multi: targets = living_player_combatants;
                                                break;
        case AbilityTargetType::team_single: targets = {living_enemy_combatants.at(std::rand() % living_enemy_combatants.size())};
                                                break;
        case AbilityTargetType::team_multi: targets = living_enemy_combatants;
                                                break;
        case AbilityTargetType::all: targets = AllLivingCombatants();
        }

        display_text_ = active_combatant_->UseAbility(*active_turn_chosen_ability_, targets);
    }

    state_ = CombatState::action_display;
}

void Combat::SetActionAndProgress(Attack* attack, Ability* ability)
{
    if (attack != NULL) {
        this->active_turn_chosen_attack_ = attack;
        this->active_turn_targets_ = LivingEnemyCombatants();

        if (attack->target_type == AttackTargetType::single) {
            this->state_ = CombatState::attack_target_selection;
        } else {
            display_text_ = active_combatant_->PerformAttack(*attack, active_turn_targets_);
            this->state_ = CombatState::action_display;
        }

    } else if (ability != NULL) {
        this->active_turn_chosen_ability_ = ability;

        switch (ability->target_type)
        {
        case AbilityTargetType::self: display_text_ = this->active_combatant_->UseAbility(*ability, {active_combatant_});
                                        this->state_ = CombatState::action_display; 
                                        break;
        case AbilityTargetType::enemy_single: this->active_turn_targets_ = LivingEnemyCombatants();
                                                this->state_ = CombatState::ability_target_selection;
                                                break;
        case AbilityTargetType::enemy_multi: display_text_ = this->active_combatant_->UseAbility(*ability, LivingEnemyCombatants());
                                                this->state_ = CombatState::action_display; 
                                                break;
        case AbilityTargetType::team_single: this->active_turn_targets_ = LivingPlayerCombatants();
                                                this->state_ = CombatState::ability_target_selection;
                                                break;
        case AbilityTargetType::team_multi: display_text_ = this->active_combatant_->UseAbility(*ability, LivingPlayerCombatants());
                                            this->state_ = CombatState::action_display; 
                                            break;
        case AbilityTargetType::all: display_text_ = this->active_combatant_->UseAbility(*ability, AllLivingCombatants());
                                        this->state_ = CombatState::action_display; 
                                        break;
        }
    } else {
        std::cout << "No attack or ability chosen.\n";
    }
}

void Combat::DistributeCombatExp()
{
    std::ostringstream os;

    for (auto combatant : LivingPlayerCombatants()) {
        os << ((PlayerCombatant*)combatant)->GainExp(sum_ec_lvl_);
    }

    this->display_text_ = os.str();
    this->state_ = CombatState::exp_gain_display;
}

void Combat::Progress(Attack* attack, Ability* ability, Combatant* target)
{
    switch (state_)
    {
    case CombatState::start:
        SetInitialCooldowns();
        ProgressToNextAction();
        break;
    case CombatState::state_reset_display:
        ProgressToNextAction();
        break;
    case CombatState::turn_start_display:
        StartTurn();
        break;
    case CombatState::action_selection:
        SetActionAndProgress(attack, ability);
        break;
    case CombatState::attack_target_selection:
        display_text_ = active_combatant_->PerformAttack(*active_turn_chosen_attack_, {target});
        this->state_ = CombatState::action_display; 
        break;
    case CombatState::ability_target_selection: 
        display_text_ = active_combatant_->UseAbility(*active_turn_chosen_ability_, {target});
        this->state_ = CombatState::action_display;
        break;
    case CombatState::action_display: 
        ProgressToNextAction();
        break;
    case CombatState::losing_screen: /*Cleanup;*/ break;
    case CombatState::winning_screen:
        DistributeCombatExp();
        break;
    case CombatState::exp_gain_display:
        std::srand(std::time(nullptr));
        this->loot_ = GenerateLoot(min_pc_lvl_, max_pc_lvl_);
        this->state_ = CombatState::loot_display;
        break;
    case CombatState::loot_display: /*Cleanup;*/ break;
    }
}