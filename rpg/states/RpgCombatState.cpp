#include "RpgCombatState.hpp"

#include "../combat/EnemyGenerator.hpp"
#include "../Map.hpp"
#include "../ecs/ecs.hpp"
#include "../Vector2D.hpp"
#include "../RpgLabel.hpp"
#include "RpgPlayState.hpp"

#define FONT_MSG "Ancient"
#define FONT_CNAME "Ancient_s"
#define FONT_CLVL "Ancient_s"
#define FONT_CSTATE "Ancient_s"
#define FONT_CHP "Ancient_s"
#define FONT_CCD "Ancient_s"

SDL_Texture* combat_arena;
SDL_Event RpgCombatState::event;
SDL_Color color_white = {255, 255, 255};

extern Manager manager;

Vector2D GetCombatantPosition(bool player_team, int number)
{
    if (player_team) {
        switch(number) {
            case 0: return Vector2D(300, 250);
            case 1: return Vector2D(175, 150);
            case 2: return Vector2D(50, 250);
            default: return Vector2D(25, 150);
        }
    } else {
        switch(number) {
            case 0: return Vector2D(549, 250);
            case 1: return Vector2D(674, 150);
            case 2: return Vector2D(799, 250);
            case 3: return Vector2D(924, 150);
            default: return Vector2D(924, 350);
        }
    }
}

RpgCombatState::RpgCombatState(std::vector<Combatant*> player_combatants, CombatArena arena)
: label_msg(RpgLabel(50, 550, "", FONT_MSG, color_white)), 
    label_combatant(RpgLabel(80, 600, "", FONT_MSG, color_white)), 
    label_action(RpgLabel(80, 650, "", FONT_MSG, color_white))
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

    RpgGame::assets->AddFont("Ancient_s", "../rpg/assets/font/ancient.ttf", 35);

    int c = 0;
    for (Combatant* player_combatant : combat.player_combatants_) {
        Vector2D pos = GetCombatantPosition(true, c++);
        RpgGame::assets->CreateCombatant(pos, player_combatant->sprite_name(), true);
        auto l_name = RpgLabel(pos.x + 37,pos.y - 70, player_combatant->name(), FONT_CNAME, color_white);
        l_name.CenterLabelHorizontally();
        auto l_lvl = RpgLabel(pos.x + 37,pos.y - 35, "Lvl. " + std::to_string(player_combatant->level()), FONT_CLVL, color_white);
        l_lvl.CenterLabelHorizontally();
        auto l_state = RpgLabel(pos.x + 37,pos.y + 100, player_combatant->state_string(), FONT_CSTATE, color_white);
        l_state.CenterLabelHorizontally();
        auto l_cooldown = RpgLabel(pos.x,pos.y + 170, "", FONT_CCD, color_white);
        auto l_hp = RpgLabel(pos.x,pos.y + 135, "HP: " + std::to_string(player_combatant->hp()), FONT_CHP, color_white);
        CombatantLabels l_combatant = {l_name, l_lvl, l_state, l_cooldown, l_hp};
        this->labels_combatants.push_back(l_combatant);
    }
    c = 0;
    for (Combatant* enemy_combatant : combat.enemy_combatants_) {
        Vector2D pos = GetCombatantPosition(false, c++);
        RpgGame::assets->CreateCombatant(pos, enemy_combatant->sprite_name(), false);
        auto l_name = RpgLabel(pos.x + 37,pos.y - 70, enemy_combatant->name(), FONT_CNAME, color_white);
        l_name.CenterLabelHorizontally();
        auto l_lvl = RpgLabel(pos.x + 37,pos.y - 35, "Lvl. " + std::to_string(enemy_combatant->level()), FONT_CLVL, color_white);
        l_lvl.CenterLabelHorizontally();
        auto l_state = RpgLabel(pos.x + 37,pos.y + 100, enemy_combatant->state_string(), FONT_CSTATE, color_white);
        l_state.CenterLabelHorizontally();
        auto l_cooldown = RpgLabel(pos.x,pos.y + 170, "", FONT_CCD, color_white);
        auto l_hp = RpgLabel(pos.x,pos.y + 135, "HP: " + std::to_string(enemy_combatant->hp()), FONT_CHP, color_white);
        CombatantLabels l_combatant = {l_name, l_lvl, l_state, l_cooldown, l_hp};
        this->labels_combatants.push_back(l_combatant);
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

static bool combat_state_changed = true;
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

void RpgCombatState::Update(RpgGame *rpgGame)
{
    manager.refresh();
	manager.update();

    rpgGame->camera.x = 0;
    rpgGame->camera.y = 0;

    if (!combat_state_changed) {
        return;
    }

    if (combat.state() != CombatState::start) {
        UpdateCombatantLabels();
    }

    switch (combat.state())
    {
    case CombatState::start:
        label_msg.setLabelText(FONT_MSG, "Enemies appear!");
        break;
    case CombatState::turn_start_display:
        label_msg.setLabelText(FONT_MSG, "It's the turn of " + combat.active_combatant()->name());
        break;
    case CombatState::action_selection:
        label_msg.setLabelText(FONT_MSG, "Select action:");
        label_combatant.setLabelText(FONT_MSG, combat.active_combatant()->name());
        break;
    case CombatState::attack_target_selection:
        label_msg.setLabelText(FONT_MSG, "Select target:");
        label_action.setLabelText(FONT_MSG, combat.active_turn_chosen_attack()->name);
        break;
    case CombatState::ability_target_selection:
        label_msg.setLabelText(FONT_MSG, "Select target:");
        label_action.setLabelText(FONT_MSG, combat.active_turn_chosen_ability()->name);
        break;
    case CombatState::action_display:
        label_combatant.setLabelText(FONT_MSG, "");
        label_action.setLabelText(FONT_MSG, "");
        if (combat.active_turn_chosen_attack() != NULL) {
            label_msg.setLabelText(FONT_MSG, combat.active_combatant()->name() + " uses " + combat.active_turn_chosen_attack()->name);
        } else if (combat.active_turn_chosen_ability() != NULL) {
            label_msg.setLabelText(FONT_MSG, combat.active_combatant()->name() + " uses " + combat.active_turn_chosen_ability()->name);
        } else {
            std::cout << "no action chosen for some reason\n";
        }
        break;
    case CombatState::state_reset_display:
        label_msg.setLabelText(FONT_MSG, "A state is reset");
        break;
    case CombatState::winning_screen:
        label_msg.setLabelText(FONT_MSG, "You win");
        break;
    case CombatState::losing_screen:
        label_msg.setLabelText(FONT_MSG, "You lose");
        break;
    case CombatState::loot_display:
        label_msg.setLabelText(FONT_MSG, "Looooooot :)");
        break;
    default:
        break;
    }
    combat_state_changed = false;
}

void RpgCombatState::UpdateCombatantLabels()
{
    int c = 0;
    for (auto player_combatant : combat.player_combatants_) {
        CombatantLabels* l_combatant = &(labels_combatants.at(c++));
        l_combatant->cooldown.setLabelText(FONT_CCD, "CD: " + std::to_string(player_combatant->cooldown_));
        l_combatant->hp.setLabelText(FONT_CHP, "HP: " + std::to_string(player_combatant->hp()));
        l_combatant->state.RealignLabelHorizontally();
        if (player_combatant->state_ != CombatantState::normal && player_combatant->state_ != CombatantState::dead) {
            l_combatant->state.setLabelText(FONT_CSTATE, player_combatant->state_string() + " (" + std::to_string(player_combatant->state_reset_countdown_) + ")");
        } else {
            l_combatant->state.setLabelText(FONT_CSTATE, player_combatant->state_string());
        }
        l_combatant->state.CenterLabelHorizontally();
    }
    for (auto enemy_combatant : combat.enemy_combatants_) {
        CombatantLabels* l_combatant = &(labels_combatants.at(c++));
        l_combatant->cooldown.setLabelText(FONT_CCD, "CD: " + std::to_string(enemy_combatant->cooldown_));
        l_combatant->hp.setLabelText(FONT_CHP, "HP: " + std::to_string(enemy_combatant->hp()));
        l_combatant->state.RealignLabelHorizontally();
        if (enemy_combatant->state_ != CombatantState::normal && enemy_combatant->state_ != CombatantState::dead) {
            l_combatant->state.setLabelText(FONT_CSTATE, enemy_combatant->state_string() + "(" + std::to_string(enemy_combatant->state_reset_countdown_) + ")");
        } else {
            l_combatant->state.setLabelText(FONT_CSTATE, enemy_combatant->state_string());
        }
        l_combatant->state.CenterLabelHorizontally();
    }
}

void RpgCombatState::Render(RpgGame *rpgGame)
{
    SDL_RenderClear(rpgGame->renderer);

	TextureManager::Draw(combat_arena, NULL, NULL, SDL_FLIP_NONE, SDL_ALPHA_OPAQUE);

	for (auto &p : player_c) {
		p->draw(SDL_ALPHA_OPAQUE);
	}

    for (auto &e : enemy_c) {
        e->draw(SDL_ALPHA_OPAQUE);
    }

    for (auto l_combatant : labels_combatants) {
        l_combatant.name.Draw();
        l_combatant.lvl.Draw();
        l_combatant.state.Draw();
        l_combatant.cooldown.Draw();
        l_combatant.hp.Draw();
    }

    this->label_msg.Draw();
    this->label_combatant.Draw();
    this->label_action.Draw();

	SDL_RenderPresent(rpgGame->renderer);
}