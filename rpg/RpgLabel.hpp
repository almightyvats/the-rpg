#ifndef RPG_LABEL_HPP
#define RPG_LABEL_HPP
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class RpgLabel {
  private:
	SDL_Rect m_position;
	std::string m_labelText;
	std::string m_labelFontId;
	SDL_Color m_color;
	SDL_Texture *m_labelTexture;

  public:
	RpgLabel(int xPosition, int yPosition, std::string labelText, std::string font, SDL_Color &color);
	~RpgLabel();

	void Draw();
	void setLabelText(std::string font, std::string labelText);
};

#endif