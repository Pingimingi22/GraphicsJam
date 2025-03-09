#pragma once
#include "PhysicsProp.h"
#include "Sprite.h"


class SpriteProp : public PhysicsProp {
protected:
	Sprite* _sprite;

	bool _isHovered = false;

public:
	SpriteProp(std::string objName,
		std::string spriteName, 
		float width, 
		float height,
		glm::vec2 position);

	virtual void ShowTooltip();
	virtual bool IsPointOverlapping(glm::vec2 point);
	virtual void ShowUI();
	virtual void Draw();
};