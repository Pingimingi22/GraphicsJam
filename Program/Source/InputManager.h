#pragma once
#include "GameObject.h"
#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "Window.h"
#include "PhysicsBody.h"

class InputManager {
private:
	std::vector<GameObject*>* gameObjects = nullptr;
	Window* window = nullptr;

public:
	static InputManager* Instance;
	InputManager(Window* window);

	glm::vec2 GetMouseScreenPoint();
	glm::vec3 GetMouseWorldPoint();

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	glm::vec3 GetCameraPosition();

	void SetDetectableGameObjects(std::vector<GameObject*>* gameObjects);

	void Update();

	PhysicsBody* BodyJoinedToMouse = nullptr;
};
