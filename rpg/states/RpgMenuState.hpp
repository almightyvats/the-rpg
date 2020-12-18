
#ifndef RPG_RPGMENUSTATE_HPP
#define RPG_RPGMENUSTATE_HPP

#include "RpgGameState.hpp"
#include "RpgMenuItem.hpp"
#include "rpg/RpgGame.hpp"
#include <map>
#include <memory>
#include <vector>

enum BUTTON_PROPS { BUTTON_HEIGHT = 200, BUTTON_WIDTH = 300 };

enum BUTTON_STATE {
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class RpgMenuItem;
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

	static SDL_Event m_event;
	static AssetManager *assets;

	std::vector<std::shared_ptr<RpgMenuItem>> m_menuItems;
	int m_currentSprite;
};

#endif // RPG_RPGMENUSTATE_HPP
