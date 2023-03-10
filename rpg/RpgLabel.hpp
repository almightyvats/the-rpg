#ifndef RPG_LABEL_HPP
#define RPG_LABEL_HPP

#include "Vector2D.hpp"
#include "RpgGame.hpp"

enum class LabelType { NONE = -1, NEWGAME = 0, LOADGAME = 1, EXIT = 2, CONTINUE = 3 };
class RpgLabel {
  private:
	SDL_Rect m_position;
	std::string m_labelText;
	std::string m_labelFontId;
	SDL_Color m_color;
	SDL_Texture *m_labelTexture;
	bool drawable;

  public:
	RpgLabel() = default;
	RpgLabel(int xPosition, int yPosition, std::string labelText, std::string font, SDL_Color &color);
	RpgLabel(LabelType type, std::string labelText, std::string font, SDL_Color &color);
	~RpgLabel();

	void Draw();
	void setLabelText(std::string font, std::string labelText);
	void setLabelColor(const SDL_Color &color);
	void getLabelDims(SDL_Rect &dims);
	void CenterLabelHorizontally();
	void RealignLabelHorizontally();
	void setLabelPos(Vector2D pos);
};

#endif