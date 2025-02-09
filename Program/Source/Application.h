#pragma once
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Application
{
public:
	Application(const std::string& name, uint32_t width, uint32_t height);
	~Application();

	void Run();

private:
	Window* m_Window;
	bool _drawPhysicsColliders = false;
};