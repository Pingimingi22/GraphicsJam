#pragma once
#include "GameObject.h"
#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "Window.h"

class InputManager {
private:
	InputManager* _instance = nullptr;
	std::vector<GameObject*>* gameObjects = nullptr;
	Window* window = nullptr;

public:
	InputManager(Window* window);
	InputManager Instance();

	glm::vec2 GetMouseScreenPoint();
	glm::vec3 GetMouseWorldPoint();

	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
	glm::vec3 GetCameraPosition();

	void SetDetectableGameObjects(std::vector<GameObject*>* gameObjects);

	void Update();
};
