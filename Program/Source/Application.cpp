#include "Application.h"
#include "Renderer.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "World.h"

#include "box2d/box2d.h"
#include "PhysicsBody.h"
#include "Player.h"

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

	Renderer renderer = Renderer();
	renderer.Init(*m_Window);
	
	b2WorldDef worldDef = b2DefaultWorldDef();

	b2Vec2 gravity;
	gravity.x = 0.0f;
	gravity.y = 0.0f;
	worldDef.gravity = gravity;

	b2WorldId worldId = b2CreateWorld(&worldDef);

	World world = World();
	world.Init(worldId);
	
	double previousTime = glfwGetTime();

	// Creating player sprite.
	SpriteAnimated* playerSprite = new SpriteAnimated(
		"Soldier-Walk.png", 
		glm::vec3(0.0f, 0.0f, 0.0f));
	playerSprite->SetFrameInfo(8, 100, 100, 1, 8);

	// Creating player physics.
	PhysicsBody playerPhysics = PhysicsBody(
		worldId, 
		b2_dynamicBody, 
		glm::vec2(0.0f));

	// Creating the player controller.
	Player player = Player(playerSprite, playerPhysics);

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
		
		// This function should *ideally* not exist in the window class and would
		// be better suited within a Renderer or Surface class. However, for the
		// purposes of this base project, it is fine as a temporary solution.
		m_Window->Clear(0.15f, 0.15f, 0.15f);

		for (int i = 0; i < world.sprites.size(); i++) {
			renderer.Draw(world.sprites[i], deltaTime);
		}

		player.Update(deltaTime, m_Window);
		renderer.Draw(*player.GetAnimatedSprite(), deltaTime);
		
		m_Window->ProcessEvents();

		m_Window->SwapBuffers();

		previousTime = currentTime;
	}
}
