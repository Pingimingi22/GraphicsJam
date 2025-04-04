#pragma once
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "box2d/box2d.h"
#include "glm/vec2.hpp"
#include <vector>
#include "GameObject.h"
#include "PhysicsProp.h"


class Application
{
public:
	Application(const std::string& name, uint32_t width, uint32_t height);
	~Application();

	void Run();

	//b2WorldId WorldId;

	static Application* Instance;

	void SpawnCircle(glm::vec2 position, 
		float radius, 
		std::vector<GameObject*>* gameObjects);

	void SpawnRectangle(glm::vec2 position, 
		float width, 
		float height, 
		std::vector<GameObject*>* gameObjects);

	void SpawnSpriteProp(std::string spritePath, 
		glm::vec2 position, 
		float width, 
		float height, 
		std::vector<GameObject*>* gameObjects);

	b2WorldId WorldId;

private:
	Window* m_Window;

	bool _drawPhysicsColliders = false;
	bool _drawPlayerColliders = false;
	bool _drawWorldColliders = false;

	/// <summary>
	/// World space position of the mouse when the right click context menu was opened.
	/// </summary>
	glm::vec2 _mouseContextWorldPosition = glm::vec2(0, 0);

	PhysicsProp* _debugProp;
};