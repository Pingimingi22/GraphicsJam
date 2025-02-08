#include "Application.h"
#include "Renderer.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "World.h"

#include "box2d/box2d.h"

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
	
	b2WorldDef worldDef = b2DefaultWorldDef();

	b2Vec2 gravity;
	gravity.x = 0.0f;
	gravity.y = -10.0f;
	worldDef.gravity = gravity;

	b2WorldId worldId = b2CreateWorld(&worldDef);

	//testSprite._position = glm::vec3(10.0f, 0.0f, 0.0f);
	World world = World();
	world.Init(worldId);
	
	double previousTime = glfwGetTime();

	SpriteAnimated player = SpriteAnimated("Soldier-Walk.png", glm::vec3(0.0f, 0.0f, 0.0f), worldId, b2_dynamicBody);
	player._scale = glm::vec3(20.0f, 20.0f, 1.0f);
	player.frames = 8;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec2 velocity = glm::vec2(0);
	float playerSpeed = 5;

	float timeStep = 1.0f / 60.0f;
	int subStepCount = 4;

	float timer = 0;
	while (!m_Window->ShouldClose())
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;

		b2World_Step(worldId, timeStep, subStepCount);

		velocity = glm::vec2(0);

		b2Vec2 force = b2Vec2();
		

		if (glfwGetKey(m_Window->m_NativeWindow, GLFW_KEY_W) == GLFW_PRESS) {
			force.y += 5;
		}
		if (glfwGetKey(m_Window->m_NativeWindow, GLFW_KEY_S) == GLFW_PRESS) {
			force.y -= 5;
		}
		if (glfwGetKey(m_Window->m_NativeWindow, GLFW_KEY_D) == GLFW_PRESS) {
			force.x += 5;
		}
		if (glfwGetKey(m_Window->m_NativeWindow, GLFW_KEY_A) == GLFW_PRESS) {
			force.x -= 5;
		}

		if (force.x < 0) {
			player._scale.x = -20;
		}
		else if(force.x > 0) {
			player._scale.x = 20;
		}


		timer += deltaTime;
		if (timer >= 0.1f) {
			timer = 0;

			if (force.x != 0)
				player.frameIndex += 1;
			else
				player.frameIndex = 0;

			if (player.frameIndex >= player.frames) {
				player.frameIndex = 0;
			}
		}

		b2Vec2 forcePoint = b2Vec2_zero;
		b2Body_ApplyForce(player._bodyId, force, forcePoint, true);

		b2Vec2 playerPosition = b2Body_GetPosition(player._bodyId);

		//player._position += glm::vec3(velocity.x, velocity.y, 0);

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
