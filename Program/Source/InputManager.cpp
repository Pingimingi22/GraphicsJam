#include "InputManager.h"
#include "imgui.h"
#include "Renderer.h"
#include "GLFW/glfw3.h"

InputManager::InputManager(Window* window)
{
	this->window = window;
}

glm::vec2 InputManager::GetMouseScreenPoint()
{
	double xPos;
	double yPos;
	glfwGetCursorPos(window->m_NativeWindow, &xPos, &yPos);
	return glm::vec2(xPos, yPos);
}

glm::vec3 InputManager::GetMouseWorldPoint()
{
	float viewportWidth = window->Width();
	float viewportHeight = window->Height();
	glm::vec2 mouseScreenSpace = GetMouseScreenPoint();
	float x = (2.0f * mouseScreenSpace.x) / viewportWidth - 1.0f;
	float y = 1.0f - (2.0f * mouseScreenSpace.y) / viewportHeight;
	float z = 1.0f;
	glm::vec3 rayNDC = glm::vec3(x, y, z);

	glm::vec4 rayClipSpace = glm::vec4(rayNDC.x, rayNDC.y, -1.0, 1.0);

	glm::vec4 rayCameraSpace = glm::inverse(GetProjectionMatrix()) * rayClipSpace;
	rayCameraSpace = glm::vec4(rayCameraSpace.x, rayCameraSpace.y, 0.0, 1.0);

	glm::vec3 rayWorldSpace = glm::inverse(GetViewMatrix()) * rayCameraSpace;
	rayWorldSpace.z = 0;

	return rayWorldSpace;
}

glm::mat4 InputManager::GetProjectionMatrix()
{
	return Renderer::Instance->camera.projectionMatrix;
}

glm::mat4 InputManager::GetViewMatrix()
{
	return Renderer::Instance->camera.GetViewMatrix();
}

glm::vec3 InputManager::GetCameraPosition()
{
	return Renderer::Instance->camera.position;
}

void InputManager::SetDetectableGameObjects(std::vector<GameObject*>* gameObjects)
{
	this->gameObjects = gameObjects;
}


void InputManager::Update()
{
	if (gameObjects == nullptr)
		return;

	for (int i = 0; i < gameObjects->size(); i++) {
		GameObject* objectRef = gameObjects[0][i];

		objectRef->ShowUI();

		if (objectRef->IsPointOverlapping(GetMouseWorldPoint())) {
			objectRef->ShowTooltip();
		}
	}

	glm::vec3 mouseWorldPos = GetMouseWorldPoint();

	ImGui::Text(std::string("Mouse world position: " + std::to_string(mouseWorldPos.x) + ", " + std::to_string(mouseWorldPos.y) + ", " + std::to_string(mouseWorldPos.z)).c_str());
}
