#include "RpgMenuItem.hpp"
#include "rpg/RpgGame.hpp"
#include "rpg/TextureManager.hpp"

RpgMenuItem::RpgMenuItem(float x, float y, float height, float width, std::string menuItemPath, ITEM_TYPE type)
{
	m_menuItemTexture = TextureManager::LoadTexture(menuItemPath);

	m_srcRect.x = x;
	m_srcRect.y = y;
	m_srcRect.w = width;
	m_srcRect.h = height;

	if (type == ITEM_TYPE::MUTE_BUTTON) {
		m_destRect.x = m_position.x = 110;
		m_destRect.y = m_position.y = 0;
		m_destRect.w = width;
		m_destRect.h = height;
	} else if (type == ITEM_TYPE::UNMUTE_BUTTON) {
		m_destRect.x = m_position.x = 101;
		m_destRect.y = m_position.y = 0;
		m_destRect.w = width;
		m_destRect.h = height;
	} else if (type == ITEM_TYPE::LOGO) {
		m_destRect.x = m_position.x = (RpgGame::SCREEN_WIDTH / 2 - width / 2);
		m_destRect.y = m_position.y = ((RpgGame::SCREEN_HEIGHT / 2 - height / 2) - 200);
		m_destRect.w = width;
		m_destRect.h = height;
	} else {
		m_destRect.x = m_position.x = 0;
		m_destRect.y = m_position.y = 0;
		m_destRect.w = width;
		m_destRect.h = height;
	}
}

RpgMenuItem::~RpgMenuItem() {}

void RpgMenuItem::getMenuItemDims(SDL_Rect &dims) const
{
	dims = {m_position.x, m_position.y, m_destRect.w, m_destRect.h};
}

void RpgMenuItem::Update() {}

void RpgMenuItem::Draw()
{
	TextureManager::Draw(m_menuItemTexture, &m_srcRect, &m_destRect, SDL_FLIP_NONE);
}