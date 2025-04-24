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
#include "CircleProp.h"
#include "RectangleProp.h"
#include "SpriteProp.h"
#include <algorithm>

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
		1.0f,
		true);

	// Creating the player controller.
	Player* player = new Player(playerSprite, playerPhysics);

	// Creating floor.
	PhysicsBody floorPhysics = PhysicsBody(
		WorldId,
		b2_staticBody,
		glm::vec2(0.0f, 5.0f),
		100.0f,
		1.0f,
		true);

	std::vector<GameObject*> gameObjects = std::vector<GameObject*>();
	gameObjects.push_back(player);
	inputManager.SetDetectableGameObjects(&gameObjects);
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::vec2 velocity = glm::vec2(0);
	float playerSpeed = 5;

	float timeStep = 1.0f / 60.0f;
	int subStepCount = 4;

	float timer = 0;

	glfwSwapInterval(1);

	float accumulator = 0.0f;

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
			renderer.Draw(world.tiles[i].sprite);
		}

		player->Update(deltaTime, m_Window);
		renderer.Draw(*player->GetAnimatedSprite(), deltaTime);
	
		if (_drawPlayerColliders) {
			renderer.DrawGizmo(player->GetPhysicsBody(), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (_drawWorldColliders) {
			renderer.DrawGizmo(floorPhysics, glm::vec3(0.95f, 0.44f, 0.91f));

			for (int i = 0; i < world.tiles.size(); i++) {
				renderer.DrawGizmo(world.tiles[i].physics, glm::vec3(0.57f, 0.92f, 0.88f));
			}
		}
		
		// Hack code to test raycast to closest vertices.
		// If at least one object has been placed down, we'll use it for our raycast
		// test.
		if (_debugProp != nullptr) {
			b2Vec2 playerPosition = { player->GetPosition().x, player->GetPosition().y };
			std::vector<b2Vec2> vertices = _debugProp->GetVerticesFacingPosition(playerPosition);

			std::vector<glm::vec3> shadowcastQuad = std::vector<glm::vec3>();
			std::vector<b2Vec2> shadowcastVertices = std::vector<b2Vec2>();

			glm::vec3 sumOfAllPositions = glm::vec3(0);
			if (vertices.size() > 0) {
				for (int i = 0; i < vertices.size(); i++) {
					b2RayCastInput testRay;
					testRay.origin = { player->GetPosition().x, player->GetPosition().y };
					testRay.translation = vertices[i] - testRay.origin;
					renderer.DrawRay(testRay, glm::vec3(0, 1, 0));

					shadowcastVertices.push_back(vertices[i]);
					glm::vec2 playerToVertexPos = glm::vec2(vertices[i].x, vertices[i].y) - player->GetPosition();
					playerToVertexPos = glm::normalize(playerToVertexPos);
					b2Vec2 playerToVertexDir = { playerToVertexPos.x, playerToVertexPos.y };

					b2Vec2 modifiedVec;
					modifiedVec = vertices[i] + playerToVertexDir * 10.0f;
					//shadowcastVertices.push_back(modifiedVec);

					sumOfAllPositions += glm::vec3(vertices[i].x, vertices[i].y, 0);
					//sumOfAllPositions += glm::vec3(modifiedVec.x, modifiedVec.y, 0);
					//renderer.DrawCircle({ shadowcastVertices[i].x, shadowcastVertices[i].y }, 0.25f, glm::vec3(1, 0, 1));
				}
				ImGui::Text( (std::string("Vertices: ") + std::to_string(vertices.size())).c_str() );
				for (int i = 0; i < vertices.size(); i++) {
					glm::vec2 playerToVertexPos = glm::vec2(vertices[i].x, vertices[i].y) - player->GetPosition();
					playerToVertexPos = glm::normalize(playerToVertexPos);
					b2Vec2 playerToVertexDir = { playerToVertexPos.x, playerToVertexPos.y };

					b2Vec2 modifiedVec;
					modifiedVec = vertices[i] + playerToVertexDir * 100.0f;
				    shadowcastVertices.push_back(modifiedVec);
					sumOfAllPositions += glm::vec3(modifiedVec.x, modifiedVec.y, 0);
				}

				// Sort the vertices.
				glm::vec3 centrePointOfVerts = glm::vec3(
					sumOfAllPositions.x / shadowcastVertices.size(),
					sumOfAllPositions.y / shadowcastVertices.size(),
					sumOfAllPositions.z / shadowcastVertices.size());

				//renderer.DrawCircle(centrePointOfVerts, 0.25f, glm::vec3(0, 1, 0));

				// Sort counterclockwise
				std::sort(shadowcastVertices.begin(), shadowcastVertices.end(), [centrePointOfVerts](auto& a, auto& b) {
					return atan2(a.y - centrePointOfVerts.y, a.x - centrePointOfVerts.x) < atan2(b.y - centrePointOfVerts.y, b.x - centrePointOfVerts.x);
					});

				for (int i = 0; i < shadowcastVertices.size(); i++) {
					int prevIndex = (i - 1 + shadowcastVertices.size()) % shadowcastVertices.size();
					int nextIndex = (i + 1) % shadowcastVertices.size();

					b2Vec2 prevToCurrent = shadowcastVertices[i] - shadowcastVertices[prevIndex];
					b2Vec2 currentToNext = shadowcastVertices[nextIndex] - shadowcastVertices[i];

					float cross = b2Cross(prevToCurrent, currentToNext);
					if (cross <= 0) {
						if (i == 0) {
							ImGui::Text("First vertex is concave!!!");
						}
						renderer.DrawCircle({ shadowcastVertices[i].x, shadowcastVertices[i].y }, 0.25f+i*0.1f, glm::vec3(0.25f+i*0.25f, 0+i*0.15f, 0));
						shadowcastQuad.push_back({ shadowcastVertices[i].x, shadowcastVertices[i].y, 0 });
					}
					else {
						renderer.DrawCircle({ shadowcastVertices[i].x, shadowcastVertices[i].y }, 0.25f+i*0.1f, glm::vec3(0.25f+i*0.25f, 0+i*0.15f, 0));
						shadowcastQuad.push_back({ shadowcastVertices[i].x, shadowcastVertices[i].y, 0 });
					}
				}

				shadowcastQuad.insert(shadowcastQuad.begin(), centrePointOfVerts);
				shadowcastQuad.push_back(glm::vec3(shadowcastQuad[1].x, shadowcastQuad[1].y, 0));

				renderer.DrawShadowcastQuad(shadowcastQuad);
			}
		}

		for (int i = 0; i < gameObjects.size(); i++) {
			gameObjects[i]->Update();
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
			b2Body_SetLinearVelocity(player->GetBodyId(), b2Vec2_zero);
			b2Body_SetTransform(player->GetBodyId(), newPlayerPos, resetRot);
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

		inputManager.Update();

		ImGui::SeparatorText("Camera");

		std::string cameraPositionString = std::to_string(Renderer::Instance->camera.position.x) + ", " +
			std::to_string(Renderer::Instance->camera.position.y) + ", " +
			std::to_string(Renderer::Instance->camera.position.z);
		ImGui::Text(std::string("Camera position " + cameraPositionString).c_str());


		if (ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("ContextMenu");
			_mouseContextWorldPosition = inputManager.GetMouseWorldPoint();
		}

		glm::vec2 spawnPosition = _mouseContextWorldPosition;
		if (ImGui::BeginPopupContextItem("ContextMenu")) {
			if (ImGui::MenuItem("Explode point")) {
				b2ExplosionDef explosion = b2DefaultExplosionDef();
				explosion.position = { spawnPosition.x, spawnPosition.y };
				explosion.radius = 8.0f;
				explosion.falloff = 25.0f;
				explosion.impulsePerLength = 40.0f;
				
				b2World_Explode(WorldId, &explosion);
			}
			if (ImGui::MenuItem("Spawn circle")) {
				SpawnCircle(spawnPosition, 2.0f, &gameObjects);
			}
			if (ImGui::MenuItem("Spawn rectangle")) {
				SpawnRectangle(spawnPosition, 4.0f, 4.0f, &gameObjects);
			}
			if (ImGui::MenuItem("Spawn crate")) {
				SpawnSpriteProp("crate.png", 
					spawnPosition,
					4.0f, 
					4.0f, 
					&gameObjects);
			}
			if (ImGui::MenuItem("Spawn barrel")) {
				SpawnSpriteProp("barrel.png",
					spawnPosition,
					2.0f,
					2.58f,
					&gameObjects);
			}
			if (ImGui::MenuItem("Spawn pumpkin")) {
				SpawnSpriteProp("pumpkin.png",
					spawnPosition,
					2.0f,
					2.58f,
					&gameObjects);
			}
			if (ImGui::MenuItem("Spawn chest")) {
				SpawnSpriteProp("chest.png",
					spawnPosition,
					2.0f,
					2.0f,
					&gameObjects);
			}
			if (ImGui::MenuItem("Spawn ladder")) {
				SpawnSpriteProp("ladder.png",
					spawnPosition,
					2.0f,
					6.0f,
					&gameObjects);
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
	CircleProp* newCircle = new CircleProp(std::string("new circle " + std::to_string(gameObjects->size())), 
		radius, 
		position);

	gameObjects->push_back(newCircle);
}

void Application::SpawnRectangle(glm::vec2 position, float width, float height, std::vector<GameObject*>* gameObjects)
{
	RectangleProp* newRectangle = new RectangleProp(std::string("new rectangle " + std::to_string(gameObjects->size())),
		width,
		height,
		position);
	gameObjects->push_back(newRectangle);
}

void Application::SpawnSpriteProp(std::string spritePath, glm::vec2 position, float width, float height, std::vector<GameObject*>* gameObjects)
{
	SpriteProp* newSpriteProp = new SpriteProp(spritePath,
		spritePath,
		width,
		height,
		position);
	gameObjects->push_back(newSpriteProp);

	_debugProp = newSpriteProp;
}
