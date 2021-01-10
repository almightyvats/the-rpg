
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

enum MUTE_BUTTON_STATE { BUTTON_SPRITE_UNMUTED = 0, BUTTON_SPRITE_MUTED = 1 };

class RpgMenuItem;
class RpgLabel;

using MenuItem = std::vector<std::shared_ptr<RpgMenuItem>>;
using LabelItem = std::shared_ptr<RpgLabel>;
using MenuLabelList = std::vector<LabelItem>;

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

	void labelPressed(LabelItem item, RpgGame *rpgGame);
	void muteBtnPressed();

  private:
	static SDL_Event m_event;

	MenuItem m_volumeButton;
	std::map<MenuItem, MUTE_BUTTON_STATE> m_muteBtnWithStates;
	std::shared_ptr<RpgMenuItem> m_menuBackgroundPtr;
	std::shared_ptr<RpgMenuItem> m_logoPtr;
	std::map<LabelItem, std::function<void(RpgGame *)>> m_buttonFucntions;
	MenuLabelList m_Labels;
	SDL_Color m_colors[2] = {{255, 255, 255}, {255, 0, 0}};
};

#endif // RPG_RPGMENUSTATE_HPP
