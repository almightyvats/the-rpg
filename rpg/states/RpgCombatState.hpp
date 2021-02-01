#include "RpgGameState.hpp"
#include "../RpgGame.hpp"
#include "../combat/Combat.hpp"
#include "../combat/EnemyCombatant.hpp"
#include "../TextureManager.hpp"
#include "../RpgLabel.hpp"

enum class CombatArena {
    grass,
};

struct CombatantLabels {
    RpgLabel name;
    RpgLabel lvl;
    RpgLabel state;
    RpgLabel cooldown;
    RpgLabel hp;
};

class RpgCombatState : public RpgGameState {

  private:
    Combat combat;
    std::vector<EnemyCombatant> enemies;

    std::vector<CombatantLabels> labels_combatants;
    RpgLabel label_msg;
    RpgLabel label_combatant;
    RpgLabel label_action;
    RpgLabel label_action_display;
    RpgLabel label_exp_display;
    std::vector<RpgLabel> labels_selection;

    void UpdateCombatantLabels();
    void ConstructSelectionMenu(std::vector<std::string> element_labels);
    void ProcessLabelClick(int index);

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