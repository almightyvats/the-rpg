#ifndef RPG_MENUITEM_HPP
#define RPG_MENUITEM_HPP

#include "RpgMenuState.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <functional>
#include <string>

class RpgMenuItem {
  private:
	float m_xPosition, m_yPosition;
	float m_height, m_width;

	std::function<void(void)> m_callbackFucntion;

	SDL_Texture *m_menuItemTexture;
	SDL_Rect m_srcRect;
	SDL_Rect m_destRect;
	SDL_Point m_position;

  public:
	RpgMenuItem(float x, float y, float height, float width, std::string menuItemPath);
	//             /*,std::function<void(void)> &callbackFucntion*/)
	~RpgMenuItem();

	bool isClickInBounds(float x, float y);
	void Draw();
	void Update();

	SDL_Point menuItemPosition() const;
};

#endif