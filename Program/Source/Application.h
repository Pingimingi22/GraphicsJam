#pragma once
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "box2d/box2d.h"
#include "glm/vec2.hpp"
#include <vector>
#include "GameObject.h"

class Application
{
public:
	Application(const std::string& name, uint32_t width, uint32_t height);
	~Application();

	void Run();

	b2WorldId WorldId;

	static Application* Instance;

	void SpawnCircle(glm::vec2 position, float radius, std::vector<GameObject*>* gameObjects);
	void SpawnRectangle(glm::vec2 position, float width, float height, std::vector<GameObject*>* gameObjects);

private:
	Window* m_Window;

	bool _drawPhysicsColliders = false;
	bool _drawPlayerColliders = false;
	bool _drawWorldColliders = false;
};