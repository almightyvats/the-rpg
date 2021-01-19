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
            case 1: return Vector2D(400, 200);
            case 2: return Vector2D(350, 200);
            case 3: return Vector2D(300, 200);
            default: return Vector2D(500, 500);
        }
    } else {
        return Vector2D(100, 100);
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

    RpgGame::assets->AddTexture("c_wraith1", "../rpg/assets/combatants/c_wraith1.png");

    int c = 0;
    for (Combatant* player_combatant : combat.player_combatants_) {
        Vector2D pos = GetCombatantPosition(false, c++);
        RpgGame::assets->CreateCombatant(pos, player_combatant->sprite_name(), true);
    }
    std::cout << c << " combatant sprites generated\n";
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