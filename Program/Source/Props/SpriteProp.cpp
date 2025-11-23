#include "SpriteProp.h"
#include "Renderer.h"
#include "Application.h"
#include "InputManager.h"
#include <algorithm>

SpriteProp::SpriteProp(std::string objName, 
	std::string spritePath, 
	float width, 
	float height,
	glm::vec2 position)
{
	_name = objName;
	PhysicsBody* newRectangleBody = new PhysicsBody(
		Application::Instance->WorldId,
		b2_dynamicBody,
		position,
		width/2,
		height/2,
		false);
	_physics = newRectangleBody;

	_sprite = new Sprite(spritePath,
		glm::vec3(0,0,0), 
		glm::vec3(width*2,height*2,1));
}

void SpriteProp::ShowTooltip()
{
	ImGui::BeginTooltip();
	ImGui::Text(std::string(_name).c_str());
	ImGui::EndTooltip();

	if (ImGui::IsMouseClicked(0)) {
		_physics->JoinObjectToMouse();
	}
}

bool SpriteProp::IsPointOverlapping(glm::vec2 point)
{
	_isHovered = false;

	b2Polygon aabb = b2Shape_GetPolygon(_physics->GetShapeId());
	glm::vec2 position = _physics->GetPosition();
	float rotation = _physics->GetRotation();

	glm::vec3 pointInAABBSpace = glm::vec3(point.x, point.y, 0);
	glm::mat4 aabbLocalToWorld = glm::translate(glm::mat4(1), { position.x, position.y, 0 }) * glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 0, 1));
	glm::mat4 aabbWorldToLocal = glm::inverse(aabbLocalToWorld);

	pointInAABBSpace = aabbWorldToLocal * glm::vec4(point.x, point.y, 0, 1);

	bool result = b2PointInPolygon({ pointInAABBSpace.x, pointInAABBSpace.y }, &aabb);
	_isHovered = result;
	return result;
}

void SpriteProp::ShowUI()
{
}

void SpriteProp::Draw()
{
	glm::vec2 position = _physics->GetPosition();
	float rotation = _physics->GetRotation();
	_sprite->SetPosition(position);
	_sprite->SetScale(glm::vec2(_physics->halfWidth * 2, _physics->halfHeight * 2));
	_sprite->SetRotation(rotation);
	Renderer::Instance->Draw(*_sprite, _isHovered);
}

void SpriteProp::DrawShadow(glm::vec2 playerPosition)
{
	std::vector<b2Vec2> vertices = GetVerticesFacingPosition({ playerPosition.x, playerPosition.y });

	std::vector<glm::vec3> shadowcastQuad = std::vector<glm::vec3>();
	std::vector<b2Vec2> shadowcastVertices = std::vector<b2Vec2>();

	glm::vec3 sumOfAllPositions = glm::vec3(0);
	if (vertices.size() > 0) {
		for (int i = 0; i < vertices.size(); i++) {
			b2RayCastInput testRay;
			testRay.origin = { playerPosition.x, playerPosition.y };
			testRay.translation = vertices[i] - testRay.origin;
			Renderer::Instance->DrawRay(testRay, glm::vec3(0, 1, 0));

			shadowcastVertices.push_back(vertices[i]);
			glm::vec2 playerToVertexPos = glm::vec2(vertices[i].x, vertices[i].y) - playerPosition;
			playerToVertexPos = glm::normalize(playerToVertexPos);
			b2Vec2 playerToVertexDir = { playerToVertexPos.x, playerToVertexPos.y };

			b2Vec2 modifiedVec;
			modifiedVec = vertices[i] + playerToVertexDir * 10.0f;
			//shadowcastVertices.push_back(modifiedVec);

			sumOfAllPositions += glm::vec3(vertices[i].x, vertices[i].y, 0);
			//sumOfAllPositions += glm::vec3(modifiedVec.x, modifiedVec.y, 0);
			//renderer.DrawCircle({ shadowcastVertices[i].x, shadowcastVertices[i].y }, 0.25f, glm::vec3(1, 0, 1));
		}
		//ImGui::Text((std::string("Vertices: ") + std::to_string(vertices.size())).c_str());
		for (int i = 0; i < vertices.size(); i++) {
			glm::vec2 playerToVertexPos = glm::vec2(vertices[i].x, vertices[i].y) - playerPosition;
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
					//ImGui::Text("First vertex is concave!!!");
				}
				Renderer::Instance->DrawCircle({ shadowcastVertices[i].x, shadowcastVertices[i].y }, 0.25f + i * 0.1f, glm::vec3(0.25f + i * 0.25f, 0 + i * 0.15f, 0));
				shadowcastQuad.push_back({ shadowcastVertices[i].x, shadowcastVertices[i].y, 0 });
			}
			else {
				Renderer::Instance->DrawCircle({ shadowcastVertices[i].x, shadowcastVertices[i].y }, 0.25f + i * 0.1f, glm::vec3(0.25f + i * 0.25f, 0 + i * 0.15f, 0));
				shadowcastQuad.push_back({ shadowcastVertices[i].x, shadowcastVertices[i].y, 0 });
			}
		}

		shadowcastQuad.insert(shadowcastQuad.begin(), centrePointOfVerts);
		shadowcastQuad.push_back(glm::vec3(shadowcastQuad[1].x, shadowcastQuad[1].y, 0));

		Renderer::Instance->DrawShadowcastQuad(shadowcastQuad);
	}
}

void SpriteProp::Update()
{
	if (_physics->GetMouseJointId().index1 != 0) {
		glm::vec3 mouseWorldPos = InputManager::Instance->GetMouseWorldPoint();
		b2MouseJoint_SetTarget(_physics->GetMouseJointId(), { mouseWorldPos.x, mouseWorldPos.y });
	}

	ResetMouseJointIfStuck();
}

void SpriteProp::ResetMouseJointIfStuck()
{
	if (_physics->GetMouseJointId().index1 != 0) {
		b2Vec2 velocity = b2Body_GetLinearVelocity(_physics->GetId());
		if (b2LengthSquared(velocity) < 0.0001f) {
			b2Body_ApplyLinearImpulse(_physics->GetId(), { 0.01f, 0.01f }, { 0,0 }, true);
		}
	}
}
