#pragma once
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "box2d/box2d.h"

class Application
{
public:
	Application(const std::string& name, uint32_t width, uint32_t height);
	~Application();

	void Run();

	b2WorldId WorldId;

	static Application* Instance;

private:
	Window* m_Window;
	bool _drawPhysicsColliders = false;
};