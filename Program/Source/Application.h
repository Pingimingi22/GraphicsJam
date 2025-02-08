#pragma once
#include "Window.h"

class Application
{
public:
	Application(const std::string& name, uint32_t width, uint32_t height);
	~Application();

	void Run();

private:
	Window* m_Window;
};