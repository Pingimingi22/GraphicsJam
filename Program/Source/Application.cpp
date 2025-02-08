#include "Application.h"
#include "Renderer.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "World.h"

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

	Renderer test = Renderer();
	test.Init(*m_Window);
	
	World world = World();
	world.Init();

	//testSprite._position = glm::vec3(10.0f, 0.0f, 0.0f);
	
	double previousTime = glfwGetTime();

	while (!m_Window->ShouldClose())
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;

		m_Window->ProcessEvents();

		// This function should *ideally* not exist in the window class and would
		// be better suited within a Renderer or Surface class. However, for the
		// purposes of this base project, it is fine as a temporary solution.
		m_Window->Clear(0.15f, 0.15f, 0.15f);

		for (int i = 0; i < world.sprites.size(); i++) {
			test.Draw(world.sprites[i], deltaTime);
		}

		m_Window->SwapBuffers();

		previousTime = currentTime;
	}
}
