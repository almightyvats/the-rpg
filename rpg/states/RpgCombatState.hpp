#include "RpgGameState.hpp"
#include "../RpgGame.hpp"
#include "../combat/Combat.hpp"
#include "../combat/EnemyCombatant.hpp"
#include "../TextureManager.hpp"

enum class CombatArena {
    grass,
};

class RpgCombatState : public RpgGameState {

  private:
    Combat combat;
    std::vector<EnemyCombatant> enemies;

  public:
    RpgCombatState(std::vector<Combatant*> player_combatants, CombatArena arena);
    ~RpgCombatState();

    static RpgCombatState &Instance(std::vector<Combatant*> player_combatants, CombatArena arena)
	{
		static RpgCombatState m_cs(player_combatants,arena);
		return m_cs;
	}

	void Pause() override;
	void Resume() override;

    void HandleEvents(RpgGame *) override;
	void Update(RpgGame *) override;
	void Render(RpgGame *) override;

    static SDL_Event event;
    enum groupLabels : std::size_t {groupPlayerCombatants, groupEnemyCombatants, groupProjectiles};
};