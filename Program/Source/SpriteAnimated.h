#pragma once
#include "Sprite.h"

class SpriteAnimated : public Sprite
{
public:
	SpriteAnimated(std::string texturePath, glm::vec3 position);

	int frames;
	int frameIndex;
	int frameWidth;
	int frameHeight;
	int rows;
	int columns;
};

