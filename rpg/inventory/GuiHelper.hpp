#pragma once
#include <string>

struct SpriteInfo {
	int x;
	int y;
	int w;
	int h;
};

class GuiHelper {
  public:
	GuiHelper(std::string xmlFile);
	~GuiHelper();

	SpriteInfo getSpriteInfo(std::string key);

  private:
	std::string m_xmlFile;
};