#include "RpgCombatState.hpp"

#include "../combat/EnemyGenerator.hpp"
#include "../Map.hpp"
#include "../ecs/ecs.hpp"
#include "../Vector2D.hpp"
#include "../RpgLabel.hpp"
#include "RpgPlayState.hpp"
#include "../RpgSoundManager.hpp"
#include "rpg/states/RpgStates.hpp"

#define FONT_MSG "Ancient"
#define FONT_SCOMB "Ancient"
#define FONT_SACTION "Ancient"
#define FONT_CNAME "Ancient_s"
#define FONT_CLVL "Ancient_s"
#define FONT_CSTATE "Ancient_s"
#define FONT_CHP "Ancient_s"
#define FONT_CCD "Ancient_s"
#define FONT_SELECTION "Ancient_s"
#define FONT_DISPLAY "Ancient_s"

#define SELECTION_MENU_MAX_ROWS 4

SDL_Texture* combat_arena;
SDL_Event RpgCombatState::event;
SDL_Color color_white = {255, 255, 255};
SDL_Color color_red = {255, 0, 0};

extern Manager manager;
const State m_state = stateCombat;

static bool combat_state_changed = true;
bool enemy_destroyed = false;
bool game_over = false;

Vector2D GetCombatantPosition(bool player_team, int number)
{
    if (player_team) {
        switch(number) {
            case 0: return Vector2D(288, 250);
            case 1: return Vector2D(163, 150);
            case 2: return Vector2D(38, 250);
            default: return Vector2D(38, 50);
        }
    } else {
        switch(number) {
            case 0: return Vector2D(537, 250);
            case 1: return Vector2D(662, 150);
            case 2: return Vector2D(787, 250);
            case 3: return Vector2D(912, 150);
            default: return Vector2D(912, 350);
        }
    }
}

std::vector<std::string> GetTargetNames(std::vector<Combatant*> targets)
{
    std::vector<std::string> names;
    for (Combatant* target : targets) {
        names.push_back(target->name());
    }
    return names;
}

std::vector<std::string> GetActionNames(const std::vector<Attack>& attacks, const std::vector<Ability>& abilities)
{
    std::vector<std::string> names;
    for (Attack attack : attacks) {
        names.push_back(attack.name);
    }
    for (Ability ability : abilities) {
        names.push_back(ability.name);
    }
    return names;
}

RpgCombatState::RpgCombatState(std::vector<Combatant*> player_combatants, CombatArena arena)
: label_msg(RpgLabel(50, 500, "", FONT_MSG, color_white)), 
    label_combatant(RpgLabel(80, 570, "", FONT_MSG, color_white)), 
    label_action(RpgLabel(80, 620, "", FONT_MSG, color_white)),
    label_action_display(RpgLabel(512, 500, "", FONT_MSG, color_white)),
    label_exp_display(RpgLabel(300, 500, "", FONT_MSG, color_white))
{

    RpgGame::assets->AddFont("Ancient_s", "../rpg/assets/font/ancient.ttf", 35);
    GenerateCombat(player_combatants, arena);
}

RpgCombatState::~RpgCombatState() = default;

void RpgCombatState::GenerateCombat(std::vector<Combatant*> player_combatants, CombatArena arena)
{
    game_over = false;
    this->enemies = GenerateSimpleEnemies(player_combatants, arena);

    std::vector<Combatant*> enemy_combatants;
    for (auto &enemy : this->enemies) {
        enemy_combatants.push_back(&enemy);
    }

    this->combat = Combat(player_combatants, enemy_combatants);

    switch(arena) {
        case CombatArena::grass: 
            combat_arena = TextureManager::LoadTexture("../rpg/assets/arenas/grass_arena_1.png");
            break;
        case CombatArena::stone: 
            combat_arena = TextureManager::LoadTexture("../rpg/assets/arenas/stone_arena_1.png");
            break;
        default:
            combat_arena = TextureManager::LoadTexture("../rpg/assets/arenas/grass_arena_1.png");
    }

    int c = 0;
    for (Combatant* player_combatant : combat.player_combatants_) {
        Vector2D pos = GetCombatantPosition(true, c++);
        RpgGame::assets->CreateCombatant(pos, player_combatant->sprite_name(), true, m_state);
        auto l_name = RpgLabel(pos.x + 50,pos.y - 70, player_combatant->name(), FONT_CNAME, color_white);
        l_name.CenterLabelHorizontally();
        auto l_lvl = RpgLabel(pos.x + 50,pos.y - 35, "Lvl. " + std::to_string(player_combatant->level()), FONT_CLVL, color_white);
        l_lvl.CenterLabelHorizontally();
        auto l_state = RpgLabel(pos.x + 50,pos.y + 100, player_combatant->state_string(), FONT_CSTATE, color_white);
        l_state.CenterLabelHorizontally();
        auto l_cooldown = RpgLabel(pos.x + 12,pos.y + 170, "", FONT_CCD, color_white);
        auto l_hp = RpgLabel(pos.x + 12,pos.y + 135, "HP: " + std::to_string(player_combatant->hp()), FONT_CHP, color_white);
        CombatantLabels l_combatant = {l_name, l_lvl, l_state, l_cooldown, l_hp};
        this->labels_combatants.push_back(l_combatant);
    }
    c = 0;
    for (Combatant* enemy_combatant : combat.enemy_combatants_) {
        Vector2D pos = GetCombatantPosition(false, c++);
        RpgGame::assets->CreateCombatant(pos, enemy_combatant->sprite_name(), false, m_state);
        auto l_name = RpgLabel(pos.x + 50,pos.y - 70, enemy_combatant->name(), FONT_CNAME, color_white);
        l_name.CenterLabelHorizontally();
        auto l_lvl = RpgLabel(pos.x + 50,pos.y - 35, "Lvl. " + std::to_string(enemy_combatant->level()), FONT_CLVL, color_white);
        l_lvl.CenterLabelHorizontally();
        auto l_state = RpgLabel(pos.x + 50,pos.y + 100, enemy_combatant->state_string(), FONT_CSTATE, color_white);
        l_state.CenterLabelHorizontally();
        auto l_cooldown = RpgLabel(pos.x + 12,pos.y + 170, "", FONT_CCD, color_white);
        auto l_hp = RpgLabel(pos.x + 12,pos.y + 135, "HP: " + std::to_string(enemy_combatant->hp()), FONT_CHP, color_white);
        CombatantLabels l_combatant = {l_name, l_lvl, l_state, l_cooldown, l_hp};
        this->labels_combatants.push_back(l_combatant);
    }
    combat_state_changed = true;
    RpgSoundManager::resumeMusic("COMBAT1");
}

auto &player_c(manager.getGroup(RpgCombatState::groupCombatPlayerCombatants));
auto &enemy_c(manager.getGroup(RpgCombatState::groupCombatEnemyCombatants));
auto &projectiles_c(manager.getGroup(RpgCombatState::groupCombatProjectiles));

void RpgCombatState::Pause() {}

void RpgCombatState::Resume() {
	RpgSoundManager::resumeMusic("COMBAT1");
}

static Attack attack_copy;
static Ability ability_copy;

void RpgCombatState::HandleEvents(RpgGame *rpgGame)
{
    if (SDL_PollEvent(&event) == 1) {
		switch (event.type) {
		case SDL_QUIT:
			rpgGame->quitGame();
			break;
        case SDL_KEYUP:
            if (combat.state() == CombatState::loot_display || combat.state() == CombatState::escape_screen || combat.state() == CombatState::losing_screen) {
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                case SDLK_RETURN:
                    CleanupCombat(rpgGame);
                    enemy_destroyed = (combat.state() == CombatState::loot_display);
                    game_over = (combat.state() == CombatState::losing_screen);
                    rpgGame->popState();
                    break;
                default:
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
            break;
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
            if (combat.state() == CombatState::action_selection || combat.state() == CombatState::attack_target_selection || combat.state() == CombatState::ability_target_selection || combat.state() == CombatState::start) {
                int mousePosX, mousePosY;
		        SDL_GetMouseState(&mousePosX, &mousePosY);
                int index = 0;
                
                for (RpgLabel & label : labels_selection) {
                    SDL_Rect label_dims;
                    label.getLabelDims(label_dims);

                    if (mousePosX >= label_dims.x && mousePosX <= (label_dims.x + label_dims.w)
                        && mousePosY >= label_dims.y && mousePosY <= (label_dims.y + label_dims.h)) {
                        label.setLabelColor(color_red);
                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            ProcessLabelClick(index);
                            combat_state_changed = true;
                            break;
                        }
                    } else {
                        label.setLabelColor(color_white);
                    }
                    index++;
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (combat.state() == CombatState::loot_display || combat.state() == CombatState::escape_screen || combat.state() == CombatState::losing_screen) {
                    CleanupCombat(rpgGame);
                    enemy_destroyed = (combat.state() == CombatState::loot_display);
                    game_over = (combat.state() == CombatState::losing_screen);
                    rpgGame->popState();
                } else {
                    combat.Progress(NULL,NULL,NULL);
                    combat_state_changed = true;
                }
            }
            break;
		default:
			break;
		}
	}
}

void RpgCombatState::ProcessLabelClick(int index) {
    if (combat.state() == CombatState::start) {
        if (index == 0) {
            combat.Progress(NULL, NULL, NULL);
        } else {
            combat.Flee();
        }
    } else if (combat.state() == CombatState::attack_target_selection || combat.state() == CombatState::ability_target_selection) {
        combat.Progress(NULL, NULL, combat.active_turn_targets().at(index));
    } else {
        int attack_labels = combat.active_turn_attacks().size();
        if (index < attack_labels) {
            attack_copy = combat.active_turn_attacks().at(index);
            combat.Progress(&attack_copy, NULL, NULL);
        } else if (index <  attack_labels + combat.active_turn_abilities().size()) {
            ability_copy = combat.active_turn_abilities().at(index - attack_labels);
            combat.Progress(NULL, &ability_copy, NULL);
        }
    }
}

void RpgCombatState::CleanupCombat(RpgGame *rpgGame)
{
    labels_combatants.clear();
    labels_selection.clear();

    for (auto pc : combat.player_combatants_) {
        pc->ResetToIdle();
    }

    for (auto p : player_c) {
        p->destroy();
    }

    for (auto e : enemy_c) {
        e->destroy();
    }
}

void RpgCombatState::Update(RpgGame *rpgGame)
{
    manager.refresh(m_state);
	manager.update(m_state);

    //rpgGame->camera.x = 0;
    //rpgGame->camera.y = 0;

    if (!combat_state_changed) {
        return;
    }

    if (combat.state() != CombatState::start) {
        UpdateCombatantLabels();
    }
    
    label_action_display.setLabelText(FONT_DISPLAY, "");
    label_exp_display.setLabelText(FONT_DISPLAY, "");
    labels_selection.clear();

    switch (combat.state())
    {
    case CombatState::start:
        label_msg.setLabelText(FONT_MSG, "Enemies appear!");
        ConstructSelectionMenu({"Engage combat", "Try to escape"});
        break;
    case CombatState::turn_start_display:
        label_msg.setLabelText(FONT_MSG, "It's the turn of " + combat.active_combatant()->name());
        break;
    case CombatState::action_selection:
        label_msg.setLabelText(FONT_MSG, "Select action:");
        label_combatant.setLabelText(FONT_SCOMB, combat.active_combatant()->name());
        ConstructSelectionMenu(GetActionNames(combat.active_turn_attacks(), combat.active_turn_abilities()));
        break;
    case CombatState::attack_target_selection:
        labels_selection.clear();
        label_msg.setLabelText(FONT_MSG, "Select target:");
        label_action.setLabelText(FONT_SACTION, combat.active_turn_chosen_attack()->name);
        ConstructSelectionMenu(GetTargetNames(combat.active_turn_targets()));
        break;
    case CombatState::ability_target_selection:
        labels_selection.clear();
        label_msg.setLabelText(FONT_MSG, "Select target:");
        label_action.setLabelText(FONT_SACTION, combat.active_turn_chosen_ability()->name);
        ConstructSelectionMenu(GetTargetNames(combat.active_turn_targets()));
        break;
    case CombatState::action_display:
        labels_selection.clear();
        label_combatant.setLabelText(FONT_SCOMB, "");
        label_action.setLabelText(FONT_SACTION, "");
        label_action_display.setLabelText(FONT_DISPLAY, combat.display_text());
        if (combat.active_turn_chosen_attack() != NULL) {
            label_msg.setLabelText(FONT_MSG, combat.active_combatant()->name() + " uses " + combat.active_turn_chosen_attack()->name);
            if (combat.active_turn_chosen_attack()->type == AttackType::melee) {
                if (combat.active_turn_chosen_attack()->target_type == AttackTargetType::single) {
                    RpgSoundManager::playEffect("MELEE_SINGLE");
                } else {
                    RpgSoundManager::playEffect("MELEE_MULTI");
                }
            } else {
                if (combat.active_turn_chosen_attack()->target_type == AttackTargetType::single) {
                    RpgSoundManager::playEffect("RANGED_SINGLE");
                } else {
                    RpgSoundManager::playEffect("RANGED_MULTI");
                }
            }
        } else if (combat.active_turn_chosen_ability() != NULL) {
            label_msg.setLabelText(FONT_MSG, combat.active_combatant()->name() + " uses " + combat.active_turn_chosen_ability()->name);
            if (combat.active_turn_chosen_ability()->effect == AbilityEffect::block) {
                RpgSoundManager::playEffect("SHIELD");
            } else if (combat.active_turn_chosen_ability()->effect == AbilityEffect::ignite) {
                RpgSoundManager::playEffect("IGNITE");
            } else if (combat.active_turn_chosen_ability()->heal > 0) {
                RpgSoundManager::playEffect("POTION");
            }
        } else {
            std::cout << "no action chosen for some reason\n";
        }
        break;
    case CombatState::state_reset_display:
        label_msg.setLabelText(FONT_MSG, combat.display_text());
        break;
    case CombatState::winning_screen:
        label_msg.setLabelText(FONT_MSG, "You win");
        break;
    case CombatState::losing_screen:
        label_msg.setLabelText(FONT_MSG, "You lose");
        break;
    case CombatState::escape_screen:
        label_msg.setLabelText(FONT_MSG, "You escaped");
        break;
    case CombatState::exp_gain_display:
        label_msg.setLabelText(FONT_MSG, "You win");
        label_exp_display.setLabelText(FONT_DISPLAY, combat.display_text());
        break;
    case CombatState::loot_display:
        label_msg.setLabelText(FONT_MSG, "You found a " + combat.loot().name() + " (Lvl. " + std::to_string(combat.loot().min_level()) + ")");
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

void RpgCombatState::ConstructSelectionMenu(std::vector<std::string> element_labels)
{
    this->labels_selection.clear();

    int row = 0;
    int column = 0;

    for (std::string label : element_labels) {
        this->labels_selection.push_back(RpgLabel(400 + (200*column), 500 + (50*row++), label, FONT_SELECTION, color_white));
        if (row >= SELECTION_MENU_MAX_ROWS) {
            row = 0;
            column++;
        }
    }
}

void RpgCombatState::Render(RpgGame *rpgGame)
{
    SDL_RenderClear(rpgGame->renderer);

	TextureManager::Draw(combat_arena, NULL, NULL, SDL_FLIP_NONE, SDL_ALPHA_OPAQUE);

	for (auto &p : player_c) {
		p->draw(SDL_ALPHA_OPAQUE, m_state);
	}

    for (auto &e : enemy_c) {
        e->draw(SDL_ALPHA_OPAQUE, m_state);
    }

    for (auto l_combatant : labels_combatants) {
        l_combatant.name.Draw();
        l_combatant.lvl.Draw();
        l_combatant.state.Draw();
        l_combatant.cooldown.Draw();
        l_combatant.hp.Draw();
    }

    for (auto l_selection : labels_selection) {
        l_selection.Draw();
    }

    this->label_msg.Draw();
    this->label_combatant.Draw();
    this->label_action.Draw();
    this->label_action_display.Draw();
    this->label_exp_display.Draw();

	SDL_RenderPresent(rpgGame->renderer);
}