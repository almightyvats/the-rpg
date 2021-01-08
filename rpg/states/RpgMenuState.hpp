
#ifndef RPG_RPGMENUSTATE_HPP
#define RPG_RPGMENUSTATE_HPP

#include "RpgGameState.hpp"
#include "RpgMenuItem.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/RpgLabel.hpp"
#include <functional>
#include <map>
#include <memory>
#include <vector>

enum BUTTON_PROPS { BUTTON_HEIGHT = 150, BUTTON_WIDTH = 300 };

enum BUTTON_STATE {
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_TOTAL = 3
};

class RpgMenuItem;
class RpgLabel;

using MenuItem = std::vector<std::shared_ptr<RpgMenuItem>>;
using LabelItem = std::shared_ptr<RpgLabel>;
using MenuLabel = std::vector<LabelItem>;

class RpgMenuState : public RpgGameState {

	RpgMenuState();
	~RpgMenuState();

  public:
	RpgMenuState(const RpgMenuState &) = delete;
	RpgMenuState &operator=(const RpgMenuState &) = delete;

	static RpgMenuState &Instance()
	{
		static RpgMenuState m_ms;
		return m_ms;
	}

	void Pause() override;
	void Resume() override;

	void HandleEvents(RpgGame *rpgGame) override;
	void Update(RpgGame *rpgGame) override;
	void Render(RpgGame *rpgGame) override;

	void buttonPressed(LabelItem item, RpgGame *rpgGame);

	static SDL_Event m_event;

	MenuItem m_playButton;
	MenuItem m_exitButton;
	std::shared_ptr<RpgMenuItem> m_menuBackgroundPtr;
	std::shared_ptr<RpgMenuItem> m_logoPtr;
	std::map<LabelItem, std::function<void(RpgGame *)>> m_buttonFucntions;
	// std::map<MenuItem, BUTTON_STATE> m_buttonsWithState;
	MenuLabel m_Labels;
	SDL_Color m_colors[2] = {{255, 255, 255}, {255, 0, 0}};
	bool selected[3] = {0, 0, 0};
};

#endif // RPG_RPGMENUSTATE_HPP
