#include "Application.h"
#include "Renderer.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "World.h"

#include "box2d/box2d.h"
#include "PhysicsBody.h"
#include "Player.h"
#include "InputManager.h"
#include "PhysicsProp.h"

Application* Application::Instance = nullptr;

Application::Application(const std::string& name, uint32_t width, uint32_t height)
{
	if (Instance == nullptr) {
		Instance = this;
	}

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
	InputManager inputManager = InputManager(m_Window);
	
	b2WorldDef worldDef = b2DefaultWorldDef();

	b2Vec2 gravity;
	gravity.x = 0.0f;
	gravity.y = -10.0f;
	worldDef.gravity = gravity;

	WorldId = b2CreateWorld(&worldDef);

	World world = World();
	//world.Init(worldId);
	world.CreateWorldFromImage("world.png");
	
	double previousTime = glfwGetTime();

	// Creating player sprite.
	SpriteAnimated* playerSprite = new SpriteAnimated(
		"Soldier-Walk.png", 
		glm::vec3(0.0f, 0.0f, 0.0f));
	playerSprite->SetFrameInfo(8, 100, 100, 1, 8);

	// Creating player physics.
	PhysicsBody playerPhysics = PhysicsBody(
		WorldId,
		b2_dynamicBody, 
		glm::vec2(20.0f, 17.0f),
		1.0f);

	// Creating the player controller.
	Player player = Player(playerSprite, playerPhysics);

	// Creating floor.
	PhysicsBody floorPhysics = PhysicsBody(
		WorldId,
		b2_staticBody,
		glm::vec2(0.0f, 5.0f),
		1.0f,
		100.0f);
	//floorPhysics.SetPosition(glm::vec2(0.0f, -10.0f));

	PhysicsBody randomCircle = PhysicsBody(
		WorldId,
		b2_dynamicBody,
		glm::vec2(10.0f, 17.0f),
		5.0f);

	PhysicsBody randomCircle2 = PhysicsBody(
		WorldId,
		b2_dynamicBody,
		glm::vec2(20.0f, 17.0f),
		2.0f);

	PhysicsBody randomCircle3 = PhysicsBody(
		WorldId,
		b2_dynamicBody,
		glm::vec2(30.0f, 17.0f),
		2.0f);

	PhysicsProp circle1 = PhysicsProp("circle 1", &randomCircle);
	PhysicsProp circle2 = PhysicsProp("circle 2", &randomCircle2);
	PhysicsProp circle3 = PhysicsProp("circle 3", &randomCircle3);

	std::vector<GameObject*> gameObjects = std::vector<GameObject*>();
	gameObjects.push_back(&player);
	gameObjects.push_back(&circle1);
	gameObjects.push_back(&circle2);
	gameObjects.push_back(&circle3);
	inputManager.SetDetectableGameObjects(&gameObjects);
	

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

		b2World_Step(WorldId, timeStep, subStepCount);
		
		// This function should *ideally* not exist in the window class and would
		// be better suited within a Renderer or Surface class. However, for the
		// purposes of this base project, it is fine as a temporary solution.
		m_Window->Clear(0.15f, 0.15f, 0.15f);

		for (int i = 0; i < world.tiles.size(); i++) {
			renderer.Draw(world.tiles[i].sprite, deltaTime);
		}

		player.Update(deltaTime, m_Window);
		renderer.Draw(*player.GetAnimatedSprite(), deltaTime);

	
		if (_drawPlayerColliders) {
			renderer.DrawGizmo(player.GetPhysicsBody(), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (_drawWorldColliders) {
			renderer.DrawGizmo(floorPhysics, glm::vec3(0.95f, 0.44f, 0.91f));

			for (int i = 0; i < world.tiles.size(); i++) {
				renderer.DrawGizmo(world.tiles[i].physics, glm::vec3(0.57f, 0.92f, 0.88f));
			}
		}
		
		for (int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Draw();
		}

		m_Window->ProcessEvents();

		if (ImGui::Button("Reset player")) {
			b2Vec2 newPlayerPos = b2Vec2();
			newPlayerPos.x = 0;
			newPlayerPos.y = 15.0f;
			b2Rot resetRot = b2Rot();
			resetRot.c = 0;
			resetRot.s = 1;
			b2Body_SetLinearVelocity(player.GetBodyId(), b2Vec2_zero);
			b2Body_SetTransform(player.GetBodyId(), newPlayerPos, resetRot);
		}

		ImGui::SeparatorText("Collider settings");

		ImGui::BeginGroup();
		if (ImGui::RadioButton("Player collider", _drawPlayerColliders)) {
			_drawPlayerColliders = !_drawPlayerColliders;
		}
		if (ImGui::RadioButton("World colliders", _drawWorldColliders)) {
			_drawWorldColliders = !_drawWorldColliders;
		}
		ImGui::EndGroup();

		if (ImGui::Button("Create joint")) {
			randomCircle.JoinObject(playerPhysics, 10);
		}
		if (ImGui::Button("Create joint 2")) {
			randomCircle2.JoinObject(randomCircle3, 10);
		}
		
		inputManager.Update();

		ImGui::SeparatorText("Camera");

		std::string cameraPositionString = std::to_string(Renderer::Instance->camera.position.x) + ", " +
			std::to_string(Renderer::Instance->camera.position.y) + ", " +
			std::to_string(Renderer::Instance->camera.position.z);
		ImGui::Text(std::string("Camera position " + cameraPositionString).c_str());


		if (ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("ContextMenu");
		}

		if (ImGui::BeginPopupContextItem("ContextMenu")) {
			if (ImGui::MenuItem("Spawn circle")) {
				SpawnCircle(inputManager.GetMouseWorldPoint(), 2.0f, &gameObjects);
			}
			if (ImGui::MenuItem("Spawn rectangle")) {
				SpawnRectangle(inputManager.GetMouseWorldPoint(), 2.0f, 2.0f, &gameObjects);
			}
			ImGui::EndPopup();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		m_Window->SwapBuffers();

		previousTime = currentTime;
	}
}

void Application::SpawnCircle(glm::vec2 position, float radius, std::vector<GameObject*>* gameObjects)
{
	PhysicsBody* newCircleBody = new PhysicsBody(
		WorldId,
		b2_dynamicBody,
		position,
		radius);

	PhysicsProp* newCircle = new PhysicsProp(std::string("new circle " + std::to_string(gameObjects->size())), newCircleBody);

	gameObjects->push_back(newCircle);
}

void Application::SpawnRectangle(glm::vec2 position, float width, float height, std::vector<GameObject*>* gameObjects)
{
	PhysicsBody* newRectangleBody = new PhysicsBody(
		WorldId,
		b2_dynamicBody,
		position, 
		width,
		height);

	PhysicsProp* newRectangle = new PhysicsProp(std::string("new rectangle " + std::to_string(gameObjects->size())), newRectangleBody);

	gameObjects->push_back(newRectangle);
}
