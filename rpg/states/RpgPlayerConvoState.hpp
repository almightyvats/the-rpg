
#ifndef RPG_RPGPLAYERCONVOSTATE_HPP
#define RPG_RPGPLAYERCONVOSTATE_HPP

#include "RpgGameState.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/RpgLabel.hpp"
#include <memory>

class RpgLabel;

using LabelItem = std::shared_ptr<RpgLabel>;
using LabelItemList = std::vector<LabelItem>;

class RpgPlayerConvoState : public RpgGameState {

	RpgPlayerConvoState();
	~RpgPlayerConvoState();

  public:
	RpgPlayerConvoState(const RpgPlayerConvoState &) = delete;
	RpgPlayerConvoState &operator=(const RpgPlayerConvoState &) = delete;

	static RpgPlayerConvoState &Instance()
	{
		static RpgPlayerConvoState m_pcs;
		return m_pcs;
	}

	void Pause() override;
	void Resume() override;

	void HandleEvents(RpgGame *rpgGame) override;
	void Update(RpgGame *rpgGame) override;
	void Render(RpgGame *rpgGame) override;

  private:
	inline static SDL_Event m_event;

	LabelItemList m_labels;
	SDL_Rect m_dialogueBox;
};

#endif