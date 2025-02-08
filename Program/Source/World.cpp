#include "World.h"
#include "glm/vec3.hpp"

void World::Init(b2WorldId worldId)
{
	for (int i = 0; i < width; i++) {
		sprites.emplace_back("grass.png", glm::vec3(i-(width/2), -10, 0), worldId, b2_staticBody);
	}
}
