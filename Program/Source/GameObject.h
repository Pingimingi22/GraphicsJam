#pragma once
#include "glm/vec2.hpp"

class GameObject {
public: 
	virtual void ShowTooltip();
	virtual glm::vec2 GetPosition();
	virtual bool IsPointOverlapping(glm::vec2 point);
	virtual void ShowUI();

	virtual void Draw();
};