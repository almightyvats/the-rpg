#pragma once
#include "RpgGameState.hpp"

class RpgInventoryState : public RpgGameState {

	RpgInventoryState();
	~RpgInventoryState();

  public:
	RpgInventoryState(const RpgInventoryState &) = delete;
	RpgInventoryState &operator=(const RpgInventoryState &) = delete;

	static RpgInventoryState &Instance()
	{
		static RpgInventoryState m_is;
		return m_is;
	}

    void Pause() override;
	void Resume() override;

	void HandleEvents(RpgGame *rpgGame) override;
	void Update(RpgGame *rpgGame) override;
	void Render(RpgGame *rpgGame) override;	

	static SDL_Event m_event;


	// MenuItem m_volumeButton;
	// std::map<MenuItem, MUTE_BUTTON_STATE> m_muteBtnWithStates;
	// std::shared_ptr<RpgMenuItem> m_menuBackgroundPtr;
	// std::shared_ptr<RpgMenuItem> m_logoPtr;
	// std::map<LabelItem, std::function<void(RpgGame *)>> m_buttonFucntions;
	// MenuLabelList m_Labels;
	// SDL_Color m_colors[2] = {{255, 255, 255}, {255, 0, 0}};
};