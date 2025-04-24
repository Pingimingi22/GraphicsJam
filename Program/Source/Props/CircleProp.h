#pragma once
#include "PhysicsProp.h"

class CircleProp : public PhysicsProp {
public:
	CircleProp(std::string name, float radius, glm::vec2 position);

	virtual bool IsPointOverlapping(glm::vec2 point);
	virtual void ShowUI();
	virtual void Draw();
	virtual void DrawShadow(glm::vec2 playerPosition);
	virtual void ShowTooltip();
};