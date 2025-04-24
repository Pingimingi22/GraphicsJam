#pragma once
#include "glm/vec2.hpp"

class Shadowcaster 
{
	virtual void DrawShadow(glm::vec2 playerPosition) = 0;
};