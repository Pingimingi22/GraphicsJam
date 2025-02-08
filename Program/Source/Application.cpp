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

	SpriteAnimated player = SpriteAnimated("Soldier-Walk.png", glm::vec3(0.0f, 0.0f, 0.0f));
	player._scale = glm::vec3(10.0f, 10.0f, 1.0f);
	player.frames = 8;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec2 velocity = glm::vec2(0);
	float playerSpeed = 5;

	while (!m_Window->ShouldClose())
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;

		velocity = glm::vec2(0);

		if (glfwGetKey(m_Window->m_NativeWindow, GLFW_KEY_W) == GLFW_PRESS) {
			velocity.y += 1.0f * deltaTime * playerSpeed;
		}
		if (glfwGetKey(m_Window->m_NativeWindow, GLFW_KEY_S) == GLFW_PRESS) {
			velocity.y -= 1.0f * deltaTime * playerSpeed;
		}
		if (glfwGetKey(m_Window->m_NativeWindow, GLFW_KEY_D) == GLFW_PRESS) {
			velocity.x += 1.0f * deltaTime * playerSpeed;
		}
		if (glfwGetKey(m_Window->m_NativeWindow, GLFW_KEY_A) == GLFW_PRESS) {
			velocity.x -= 1.0f * deltaTime * playerSpeed;
		}

		if (velocity.x < 0) {
			player._scale.x = -10;
		}
		else if(velocity.x > 0) {
			player._scale.x = 10;
		}

		player._position += glm::vec3(velocity.x, velocity.y, 0);

		// This function should *ideally* not exist in the window class and would
		// be better suited within a Renderer or Surface class. However, for the
		// purposes of this base project, it is fine as a temporary solution.
		m_Window->Clear(0.15f, 0.15f, 0.15f);

		for (int i = 0; i < world.sprites.size(); i++) {
			test.Draw(world.sprites[i], deltaTime);
		}
		test.Draw(player, deltaTime);
		
		m_Window->ProcessEvents();

		m_Window->SwapBuffers();

		previousTime = currentTime;
	}
}
