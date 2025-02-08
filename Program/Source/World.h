#pragma once
#include "glm/vec2.hpp"
#include "Sprite.h"
#include <vector>
#include "box2d/box2d.h"

class World
{
public:
	int width = 85;
	//int height;

	glm::vec2 worldCentre;

	std::vector<Sprite> sprites;
	
	void Init(b2WorldId worldId);
};

