#include "Application.h"

Application::Application(const std::string& name, uint32_t width, uint32_t height)
{
	m_Window = new Window(name, width, height);
}

Application::~Application()
{
	delete m_Window;
}

void Application::Run()
{
	// Early out if the window failed to create.
	if (!m_Window->Initialised()) return;

	while (!m_Window->ShouldClose())
	{
		m_Window->ProcessEvents();

		// This function should *ideally* not exist in the window class and would
		// be better suited within a Renderer or Surface class. However, for the
		// purposes of this base project, it is fine as a temporary solution.
		m_Window->Clear(0.15f, 0.15f, 0.15f);

		m_Window->SwapBuffers();
	}
}
