#ifndef RPG_MENUITEM_HPP
#define RPG_MENUITEM_HPP

#include "RpgMenuState.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <functional>
#include <string>

enum class ITEM_TYPE { MUTE_BUTTON = 0, UNMUTE_BUTTON = 1, LOGO = 2, BACKGROUND = 3 };
class RpgMenuItem {
  private:
	float m_xPosition, m_yPosition;
	float m_height, m_width;

	std::function<void(void)> m_callbackFunction;

	SDL_Texture *m_menuItemTexture;
	SDL_Rect m_srcRect;
	SDL_Rect m_destRect;
	SDL_Point m_position;

  public:
	RpgMenuItem(float x, float y, float height, float width, std::string menuItemPath, ITEM_TYPE type);
	~RpgMenuItem();

	void Draw();
	void Update();

	void getMenuItemDims(SDL_Rect &dims) const;
};

#endif