#include "RpgLabel.hpp"
#include "TextureManager.hpp"
#include "states/RpgPlayState.hpp"

RpgLabel::RpgLabel(int xPosition, int yPosition, std::string labelText, std::string fontId, SDL_Color &color)
    : m_labelText(labelText), m_labelFontId(fontId), m_color(color), m_labelTexture(nullptr), drawable(false)
{
	m_position.x = m_position.x = xPosition;
	m_position.y = m_position.y = yPosition;

	setLabelText(fontId, labelText);
}

RpgLabel::RpgLabel(LabelType type, std::string labelText, std::string fontId, SDL_Color &color)
    : m_labelText(labelText), m_labelFontId(fontId), m_color(color), m_labelTexture(nullptr), drawable(false)
{
	setLabelText(fontId, labelText);

	if (type == LabelType::CONTINUE) {
		m_position.x = (RpgGame::SCREEN_WIDTH / 2 - m_position.w / 2);
		m_position.y = ((RpgGame::SCREEN_HEIGHT / 2 - m_position.h / 2) + 100);
	} else if (type == LabelType::NEWGAME) {
		m_position.x = (RpgGame::SCREEN_WIDTH / 2 - m_position.w / 2);
		m_position.y = ((RpgGame::SCREEN_HEIGHT / 2 - m_position.h / 2) + 155);
	} else if (type == LabelType::LOADGAME) {
		m_position.x = (RpgGame::SCREEN_WIDTH / 2 - m_position.w / 2);
		m_position.y = ((RpgGame::SCREEN_HEIGHT / 2 - m_position.h / 2) + 210);
	} else if (type == LabelType::EXIT) {
		m_position.x = (RpgGame::SCREEN_WIDTH / 2 - m_position.w / 2);
		m_position.y = ((RpgGame::SCREEN_HEIGHT / 2 - m_position.h / 2) + 265);
	}
}

RpgLabel::~RpgLabel() {}

void RpgLabel::setLabelText(std::string fontId, std::string labelText)
{

	auto fontPtr = RpgGame::assets->GetFont(fontId);
	if (fontPtr == nullptr) {
		std::cout << "Rpglabel not getting font\n";
	} else {
		SDL_Surface *labelSurface = TTF_RenderText_Blended_Wrapped(fontPtr, labelText.c_str(), m_color, 2000);
		
		SDL_Texture *old_texture = nullptr;
		if (drawable) {
			old_texture = m_labelTexture;
		}

		m_labelTexture = SDL_CreateTextureFromSurface(RpgGame::renderer, labelSurface);

		if (drawable && old_texture != nullptr && old_texture != m_labelTexture) {
			SDL_DestroyTexture(old_texture);
		}

		SDL_FreeSurface(labelSurface);
		labelSurface = nullptr;
		SDL_QueryTexture(m_labelTexture, nullptr, nullptr, &m_position.w, &m_position.h);
		drawable = true;
	}
}

void RpgLabel::Draw()
{
	if (drawable) {
		TextureManager::Draw(m_labelTexture, nullptr, &m_position, SDL_FLIP_NONE);
	}
}

void RpgLabel::setLabelColor(const SDL_Color &color)
{
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

void RpgLabel::CenterLabelHorizontally()
{
	m_position.x -= m_position.w / 2;
}

void RpgLabel::RealignLabelHorizontally()
{
	m_position.x += m_position.w / 2;
}

void RpgLabel::setLabelPos(Vector2D pos)
{
	m_position.x = pos.x;
	m_position.y = pos.y;
}