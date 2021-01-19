#pragma once

class SpriteSheet {
  public:
	SpriteSheet() {}
	SpriteSheet(int columns, int spriteWidth, int spriteHeight, int spriteGapX, int spriteGapY)
	{
		this->columns = columns;
		this->spriteWidth = spriteWidth;
		this->spriteHeight = spriteHeight;
		this->spriteGapX = spriteGapX;
		this->spriteGapY = spriteGapY;
	};
	~SpriteSheet(){};

	int columns = 0;
	int spriteWidth = 32;
	int spriteHeight = 32;
	int spriteGapX = 0;
	int spriteGapY = 0;
};