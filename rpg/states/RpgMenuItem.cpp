#include "RpgMenuItem.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/TextureManager.hpp"

RpgMenuItem::RpgMenuItem(float x, float y, float height, float width, std::string menuItemPath
                         /*,std::function<void(void)> &callbackFucntion*/)
//, m_callbackFucntion(callbackFucntion)
{
	m_menuItemTexture = TextureManager::LoadTexture(menuItemPath);

	m_srcRect.x = x;
	m_srcRect.y = y;
	m_srcRect.w = width;
	m_srcRect.h = height;

	m_destRect.x = m_position.x = 0;
	m_destRect.y = m_position.y = 0;
	m_destRect.w = width;
	m_destRect.h = height;
}

RpgMenuItem::~RpgMenuItem() {}

SDL_Point RpgMenuItem::menuItemPosition() const
{
	return m_position;
}

bool RpgMenuItem::isClickInBounds(float x, float y)
{
	if (m_xPosition >= x && m_xPosition <= x + m_width) {
		if (m_yPosition >= y && m_yPosition <= y + m_height) {
			return true;
		}
	}
	return false;
}

void RpgMenuItem::Update() {}

void RpgMenuItem::Draw()
{
	TextureManager::Draw(m_menuItemTexture, m_srcRect, m_destRect, SDL_FLIP_NONE);
}