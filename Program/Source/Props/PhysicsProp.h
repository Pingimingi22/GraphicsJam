#pragma once
#include "GameObject.h"
#include <string>
#include "PhysicsBody.h"
#include "imgui.h"

class PhysicsProp : public GameObject {
protected:
	PhysicsBody* _physics = nullptr;
	std::string _name = "unnamed object";
	bool _popupShowing = false;

public:
	virtual void ShowTooltip() = 0;
	virtual bool IsPointOverlapping(glm::vec2 point) = 0;
	virtual void ShowUI() = 0;
	virtual void Draw() = 0;

	std::vector<b2Vec2> GetVerticesFacingPosition(b2Vec2 direction);
};