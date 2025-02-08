#pragma once
#include "Sprite.h"
#include "box2d/box2d.h"

class SpriteAnimated : public Sprite
{
public:
	SpriteAnimated(std::string texturePath, glm::vec3 position, b2WorldId worldId, b2BodyType type);

	int frames;
	int frameIndex;
	int frameWidth;
	int frameHeight;
	int rows;
	int columns;
};

