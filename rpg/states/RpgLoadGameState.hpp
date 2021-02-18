
#ifndef RPG_RPGLOADGAMESTATE_HPP
#define RPG_RPGLOADGAMESTATE_HPP

#include "RpgGameState.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/RpgLabel.hpp"
#include <memory>

class RpgLabel;

using LabelItem = std::shared_ptr<RpgLabel>;
using LabelItemList = std::vector<LabelItem>;

class RpgLoadGameState : public RpgGameState {

	RpgLoadGameState();
	~RpgLoadGameState();

  public:
	RpgLoadGameState(const RpgLoadGameState &) = delete;
	RpgLoadGameState &operator=(const RpgLoadGameState &) = delete;

	static RpgLoadGameState &Instance()
	{
		static RpgLoadGameState m_pcs;
		return m_pcs;
	}

	void Pause() override;
	void Resume() override;

	void HandleEvents(RpgGame *rpgGame) override;
	void Update(RpgGame *rpgGame) override;
	void Render(RpgGame *rpgGame) override;
	void labelPressed(LabelItem item, RpgGame *rpgGame);

  private:
	inline static SDL_Event m_event;

	LabelItem headingLabel;
	LabelItemList m_labels;
	SDL_Rect m_dialogueBox1;
	SDL_Rect m_dialogueBox2;
	std::map<LabelItem, std::function<void(RpgGame *)>> m_buttonFunctions;
	SDL_Color m_colors[2] = {{255, 255, 255}, {255, 0, 0}};
};

#endif