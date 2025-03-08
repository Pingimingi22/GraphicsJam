#pragma once
#include "GameObject.h"
#include <string>
#include "PhysicsBody.h"
#include "imgui.h"

class PhysicsProp : public GameObject {
private:
	PhysicsBody* physics = nullptr;
	std::string name = "unnamed object";

	bool popupShowing = false;

public:
	PhysicsProp(std::string name, PhysicsBody* body);

	void ShowTooltip();
	bool IsPointOverlapping(glm::vec2 point) override;
	void ShowUI() override;
	void Draw() override;
};