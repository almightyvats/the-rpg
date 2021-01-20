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

void RpgCombatState::HandleEvents(RpgGame *rpgGame) {
    if (SDL_PollEvent(&event) == 1) {
		switch (event.type) {
		case SDL_QUIT:
			rpgGame->quitGame();
			break;
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