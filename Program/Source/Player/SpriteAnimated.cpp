#include "SpriteAnimated.h"

SpriteAnimated::SpriteAnimated(
	std::string texturePath, 
	glm::vec3 position) : Sprite(texturePath, position)
{
	Sprite(texturePath, position);
}

void SpriteAnimated::SetFrameInfo(int totalFrames, int frameWidth, int frameHeight, int rows, int columns)
{
	_frames = totalFrames;
	_frameWidth = frameWidth;
	_frameHeight = frameHeight;
	_rows = rows;
	_columns = columns;
}

int SpriteAnimated::GetCurrentFrameIndex()
{
	return _frameIndex;
}

void SpriteAnimated::SetCurrentFrameIndex(int frame)
{
	_frameIndex = frame % (_frames);
}
