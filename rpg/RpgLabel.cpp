#include "RpgLabel.hpp"
#include "AssetManager.hpp"
#include "RpgGame.hpp"
#include "TextureManager.hpp"
#include "states/RpgPlayState.hpp"
#include <iostream>

RpgLabel::RpgLabel(int xPosition, int yPosition, std::string labelText, std::string fontId, SDL_Color &color)
    : m_labelText(labelText), m_labelFontId(fontId), m_color(color)
{
	m_position.x = xPosition;
	m_position.y = yPosition;

	setLabelText(fontId, labelText);
}

RpgLabel::~RpgLabel() {}

void RpgLabel::setLabelText(std::string fontId, std::string labelText)
{
	auto fontPtr = RpgPlayState::assets->GetFont(fontId);
	if (fontPtr == nullptr) {
		std::cout << "Rpglabel not getting font\n";
	}

	SDL_Surface *labelSurface = TTF_RenderText_Solid(fontPtr, labelText.c_str(), m_color);
	m_labelTexture = SDL_CreateTextureFromSurface(RpgGame::renderer, labelSurface);

	SDL_FreeSurface(labelSurface);
	SDL_QueryTexture(m_labelTexture, nullptr, nullptr, &m_position.w, &m_position.h);
}

void RpgLabel::Draw()
{
	TextureManager::Draw(m_labelTexture, nullptr, &m_position, SDL_FLIP_NONE);
}
