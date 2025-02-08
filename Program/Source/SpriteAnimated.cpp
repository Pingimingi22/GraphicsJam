#include "SpriteAnimated.h"

SpriteAnimated::SpriteAnimated(std::string texturePath, glm::vec3 position, b2WorldId worldId, b2BodyType type) : Sprite(texturePath, position, worldId, type)
{
	Sprite(texturePath, position, worldId, type);

	frameIndex = 1;
}
