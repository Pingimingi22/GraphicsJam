#include "RectangleProp.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include "Application.h"
#include "Renderer.h"

RectangleProp::RectangleProp(std::string name, 
	float width, 
	float height, glm::vec2 position)
{
	_name = name;
	PhysicsBody* newRectangleBody = new PhysicsBody(
		Application::Instance->WorldId,
		b2_dynamicBody,
		position,
		width/2,
		height/2,
		false);
	_physics = newRectangleBody;
}

bool RectangleProp::IsPointOverlapping(glm::vec2 point)
{
	b2Polygon aabb = b2Shape_GetPolygon(_physics->GetShapeId());
	glm::vec2 position = _physics->GetPosition();

	glm::vec3 pointInAABBSpace = glm::vec3(point.x, point.y, 0);
	glm::mat4 aabbLocalToWorld = glm::translate(glm::mat4(1), { position.x, position.y, 0 });
	glm::mat4 aabbWorldToLocal = glm::inverse(aabbLocalToWorld);

	pointInAABBSpace = aabbWorldToLocal * glm::vec4(point.x, point.y, 0, 1);

	return b2PointInPolygon({ pointInAABBSpace.x, pointInAABBSpace.y }, &aabb);
}

void RectangleProp::ShowUI()
{
	if (_popupShowing) {
		//ImGui::Begin("Test menu", &popupShowing);
		ImGui::OpenPopup(_name.c_str());

		if (ImGui::IsAnyMouseDown()) {
			_popupShowing = false;
		}
		//ImGui::SetNextWindowPos(ImGui::GetMousePos());
	}

	if (ImGui::BeginPopup(_name.c_str())) {

		glm::vec2 pos = _physics->GetPosition();
		ImGui::Text(std::string("Position: " + std::to_string(pos.x) + ", " + std::to_string(pos.y)).c_str());

		b2AABB aabb = b2Shape_GetAABB(_physics->GetShapeId());
		b2Vec2 aabbCentre = b2AABB_Center(aabb);
		b2Vec2 aabbsExtents = b2AABB_Extents(aabb);
		ImGui::Text(std::string("Half extents: " + std::to_string(aabbsExtents.x) + ", " + std::to_string(aabbsExtents.y)).c_str());

		ImGui::EndPopup();
	}
}

void RectangleProp::Draw()
{
	Renderer::Instance->DrawGizmo(*_physics, glm::vec3(1.0f, 0.0f, 0.0f));
}

void RectangleProp::ShowTooltip()
{
	ImGui::BeginTooltip();
	ImGui::Text(std::string(_name + " physics prop").c_str());
	if (ImGui::IsMouseClicked(1)) {
		_popupShowing = true;
	}
	ImGui::EndTooltip();
}
