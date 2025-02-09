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

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window->m_NativeWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
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
	gravity.y = -10.0f;
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
		glm::vec2(0.0f),
		1.0f,
		1.0f);

	// Creating the player controller.
	Player player = Player(playerSprite, playerPhysics);

	// Creating floor.
	PhysicsBody floorPhysics = PhysicsBody(
		worldId,
		b2_staticBody,
		glm::vec2(0.0f),
		100.0f,
		0.4f);
	floorPhysics.SetPosition(glm::vec2(0.0f, -10.0f));

	PhysicsBody randomCircle = PhysicsBody(
		worldId,
		b2_dynamicBody,
		glm::vec2(0.0f, 10.0f),
		5.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec2 velocity = glm::vec2(0);
	float playerSpeed = 5;

	float timeStep = 1.0f / 60.0f;
	int subStepCount = 4;

	float timer = 0;
	while (!m_Window->ShouldClose())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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

		if (_drawPhysicsColliders) {
			renderer.DrawGizmo(player.GetPhysicsBody(), glm::vec3(0.0f, 1.0f, 0.0f));
			renderer.DrawGizmo(floorPhysics, glm::vec3(0.95f, 0.44f, 0.91f));
		}

		renderer.DrawGizmo(randomCircle, glm::vec3(1.0f, 0.0f, 0.0f));
		
		m_Window->ProcessEvents();

		if (ImGui::Button("Reset player")) {
			b2Rot resetRot = b2Rot();
			resetRot.c = 0;
			resetRot.s = 1;
			b2Body_SetLinearVelocity(player.GetBodyId(), b2Vec2_zero);
			b2Body_SetTransform(player.GetBodyId(), b2Vec2_zero, resetRot);
		}

		if (ImGui::RadioButton("Physics colliders", _drawPhysicsColliders)) {
			_drawPhysicsColliders = !_drawPhysicsColliders;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		m_Window->SwapBuffers();

		previousTime = currentTime;
	}
}
