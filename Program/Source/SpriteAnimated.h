#pragma once
#include "Sprite.h"
#include "box2d/box2d.h"

class SpriteAnimated : public Sprite
{
private:
	int _frames = -1;
	int _frameIndex = -1;
	int _frameWidth = -1;
	int _frameHeight = -1;
	int _rows = -1;
	int _columns = -1;

public:
	SpriteAnimated(
		std::string texturePath, 
		glm::vec3 position);

	void SetFrameInfo(
		int totalFrames,
		int frameWidth,
		int frameHeight,
		int rows,
		int columns);

	int GetCurrentFrameIndex();
	void SetCurrentFrameIndex(int frame);

	// Timer which you increment in an update loop. This helps
	// manage which flipbook frame you want to show.
	float animationTimer = 0;
};

