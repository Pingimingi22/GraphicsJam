#pragma once
#include "PhysicsProp.h"

class RectangleProp : public PhysicsProp {
public:
	RectangleProp(std::string name, float width, float height, glm::vec2 position);

	virtual bool IsPointOverlapping(glm::vec2 point);
	virtual void ShowUI();
	virtual void Draw();
	virtual void DrawShadow(glm::vec2 playerPosition);
	virtual void ShowTooltip();
};