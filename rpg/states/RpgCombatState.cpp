#include "RpgCombatState.hpp"

#include "../combat/EnemyGenerator.hpp"
#include "../Map.hpp"
#include "../ecs/ecs.hpp"
#include "../Vector2D.hpp"
#include "RpgPlayState.hpp"

SDL_Texture* combat_arena;
SDL_Event RpgCombatState::event;

extern Manager manager;

Vector2D GetCombatantPosition(bool player_team, int number)
{
    if (player_team) {
        switch(number) {
            case 0: return Vector2D(325, 250);
            case 1: return Vector2D(225, 150);
            case 2: return Vector2D(125, 250);
            default: return Vector2D(25, 150);
        }
    } else {
        switch(number) {
            case 0: return Vector2D(624, 250);
            case 1: return Vector2D(724, 150);
            case 2: return Vector2D(824, 250);
            case 3: return Vector2D(924, 150);
            default: return Vector2D(924, 350);
        }
    }
}

RpgCombatState::RpgCombatState(std::vector<Combatant*> player_combatants, CombatArena arena)
{
    this->enemies = GenerateSimpleEnemies(player_combatants);

    std::vector<Combatant*> enemy_combatants;
    for (auto &enemy : this->enemies) {
        enemy_combatants.push_back(&enemy);
    }

    this->combat = Combat(player_combatants, enemy_combatants);

    switch(arena) {
        case CombatArena::grass: 
            combat_arena = TextureManager::LoadTexture("../rpg/assets/arenas/grass_arena_1.png");
            break;
        default:
            combat_arena = TextureManager::LoadTexture("../rpg/assets/arenas/grass_arena_1.png");
    }

    int c = 0;
    for (Combatant* player_combatant : combat.player_combatants_) {
        Vector2D pos = GetCombatantPosition(true, c++);
        RpgGame::assets->CreateCombatant(pos, player_combatant->sprite_name(), true);
    }
    c = 0;
    for (Combatant* enemy_combatant : combat.enemy_combatants_) {
        Vector2D pos = GetCombatantPosition(false, c++);
        RpgGame::assets->CreateCombatant(pos, enemy_combatant->sprite_name(), false);
    }
}

RpgCombatState::~RpgCombatState() = default;

auto &player_c(manager.getGroup(RpgCombatState::groupPlayerCombatants));
auto &enemy_c(manager.getGroup(RpgCombatState::groupEnemyCombatants));
auto &projectiles_c(manager.getGroup(RpgCombatState::groupProjectiles));

void RpgCombatState::Pause() {

}

void RpgCombatState::Resume() {

}

static bool combat_state_changed;
Attack attack_copy;
Ability ability_copy;

void RpgCombatState::HandleEvents(RpgGame *rpgGame) {
    if (SDL_PollEvent(&event) == 1) {
		switch (event.type) {
		case SDL_QUIT:
			rpgGame->quitGame();
			break;
        case SDL_KEYUP:
            if (combat.state() == CombatState::action_selection) {
                switch (event.key.keysym.sym)
                {
                case SDLK_1: 
                    attack_copy = combat.active_turn_attacks().at(0);
                    combat.Progress(&attack_copy,NULL,NULL);
                    combat_state_changed = true;
                    break;
                case SDLK_2: 
                    ability_copy = combat.active_turn_abilities().at(0);
                    combat.Progress(NULL,&ability_copy,NULL);
                    combat_state_changed = true;
                    break;
                }
            } else if (combat.state() == CombatState::attack_target_selection || combat.state() == CombatState::ability_target_selection) {
                switch (event.key.keysym.sym)
                {
                case SDLK_1:
                    combat.Progress(NULL,NULL,combat.active_turn_targets().at(0));
                    combat_state_changed = true;
                    break;
                }
            } else {
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    combat.Progress(NULL,NULL,NULL);
                    combat_state_changed = true;
                    break;
                case SDLK_RETURN:
                    combat.Progress(NULL,NULL,NULL);
                    combat_state_changed = true;
                    break;
                default:
                    break;
                }
            }
		default:
			break;
		}
	}
}

void RpgCombatState::Update(RpgGame *rpgGame) {
    manager.refresh();
	manager.update();

    rpgGame->camera.x = 0;
    rpgGame->camera.y = 0;

    if (!combat_state_changed) {
        return;
    }

    switch (combat.state())
    {
    case CombatState::start:
        std::cout << "Combat has started\n";
        break;
    case CombatState::turn_start_display:
        std::cout << "It's the turn of " << combat.active_combatant()->name() << "\n";
        break;
    case CombatState::action_selection:
        std::cout << "Choose action for " << combat.active_combatant()->name() << "\n";
        break;
    case CombatState::attack_target_selection:
        /*if (combat.active_turn_chosen_attack() != NULL)
            std::cout << "Choose target for " << combat.active_turn_chosen_attack()->name << "\n";*/
        for (auto target : combat.active_turn_targets()) {
            std::cout << target->name() << "\n";
        }
        std::cout << attack_copy.name << "\n";
        break;
    case CombatState::ability_target_selection:
        std::cout << "Choose target for " << combat.active_turn_chosen_ability()->name << "\n";
        break;
    case CombatState::action_display:
        /*if (combat.active_turn_chosen_attack() != NULL) {
            std::cout << combat.active_combatant()->name() << " uses " << combat.active_turn_chosen_attack()->name << "\n";
        } else if (combat.active_turn_chosen_attack() != NULL) {
            std::cout << combat.active_combatant()->name() << " uses " << combat.active_turn_chosen_ability()->name << "\n";
        } else {
            std::cout << "no action chosen for some reason\n";
        }*/
        break;
    case CombatState::state_reset_display:
        std::cout << "a state is reset\n";
        break;
    case CombatState::winning_screen:
        std::cout << "You won\n";
        break;
    case CombatState::losing_screen:
        std::cout << "You lose\n";
        break;
    case CombatState::loot_display:
        std::cout << "You loot\n";
        break;
    default:
        break;
    }
    combat_state_changed = false;
}

void RpgCombatState::Render(RpgGame *rpgGame) {
    SDL_RenderClear(rpgGame->renderer);

	TextureManager::Draw(combat_arena, NULL, NULL, SDL_FLIP_NONE, SDL_ALPHA_OPAQUE);

	for (auto &p : player_c) {
		p->draw(SDL_ALPHA_OPAQUE);
	}

    for (auto &e : enemy_c) {
        e->draw(SDL_ALPHA_OPAQUE);
    }

	SDL_RenderPresent(rpgGame->renderer);
}