#include "RpgLabel.hpp"
#include "AssetManager.hpp"
#include "RpgGame.hpp"
#include "TextureManager.hpp"
#include "states/RpgPlayState.hpp"
#include <iostream>

RpgLabel::RpgLabel(int xPosition, int yPosition, std::string labelText, std::string fontId, SDL_Color &color)
    : m_labelText(labelText), m_labelFontId(fontId), m_color(color)
{
	m_position.x = m_position.x = xPosition;
	m_position.y = m_position.y = yPosition;

	setLabelText(fontId, labelText);
}

RpgLabel::RpgLabel(LabelType type, std::string labelText, std::string fontId, SDL_Color &color)
    : m_labelText(labelText), m_labelFontId(fontId), m_color(color)
{
	setLabelText(fontId, labelText);

	if (type == LabelType::NEWGAME) {
		m_position.x = (RpgGame::SCREEN_WIDTH / 2 - m_position.w / 2);
		m_position.y = ((RpgGame::SCREEN_HEIGHT / 2 - m_position.h / 2) + 100);
	} else if (type == LabelType::LOADGAME) {
		m_position.x = (RpgGame::SCREEN_WIDTH / 2 - m_position.w / 2);
		m_position.y = ((RpgGame::SCREEN_HEIGHT / 2 - m_position.h / 2) + 155);
	} else if (type == LabelType::EXIT) {
		m_position.x = (RpgGame::SCREEN_WIDTH / 2 - m_position.w / 2);
		m_position.y = ((RpgGame::SCREEN_HEIGHT / 2 - m_position.h / 2) + 210);
	}
}

RpgLabel::~RpgLabel() {}

void RpgLabel::setLabelText(std::string fontId, std::string labelText)
{
	auto fontPtr = RpgGame::assets->GetFont(fontId);
	if (fontPtr == nullptr) {
		std::cout << "Rpglabel not getting font\n";
	} else {
		SDL_Surface *labelSurface = TTF_RenderText_Solid(fontPtr, labelText.c_str(), m_color);
		m_labelTexture = SDL_CreateTextureFromSurface(RpgGame::renderer, labelSurface);

		SDL_FreeSurface(labelSurface);
		labelSurface = nullptr;
		SDL_QueryTexture(m_labelTexture, nullptr, nullptr, &m_position.w, &m_position.h);
	}
}

void RpgLabel::Draw()
{
	TextureManager::Draw(m_labelTexture, nullptr, &m_position, SDL_FLIP_NONE);
}

void RpgLabel::setLabelColor(const SDL_Color &color)
{
	SDL_DestroyTexture(m_labelTexture);
	m_color = color;
	setLabelText(m_labelFontId, m_labelText);
}

void RpgLabel::getLabelDims(SDL_Rect &dims)
{
	dims = {
	    m_position.x,
	    m_position.y,
	    m_position.w,
	    m_position.h,
	};
}