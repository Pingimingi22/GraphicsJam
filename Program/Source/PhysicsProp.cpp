#include "PhysicsProp.h"
#include "glm/common.hpp"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "Renderer.h"

PhysicsProp::PhysicsProp(std::string name, PhysicsBody* body)
{
	physics = body;
	this->name = name;
}

void PhysicsProp::ShowTooltip()
{
	ImGui::BeginTooltip();
	ImGui::Text(std::string(name + " physics prop").c_str());
	if (ImGui::IsMouseClicked(1)) {
		popupShowing = true;
	}
	ImGui::EndTooltip();
}

bool PhysicsProp::IsPointOverlapping(glm::vec2 point)
{
	if (physics->shapeType == b2ShapeType::b2_circleShape) {
		b2Circle circle = b2Shape_GetCircle(physics->GetShapeId());
		glm::vec2 position = physics->GetPosition();

		glm::vec3 pointInCircleSpace = glm::vec3(point.x, point.y, 0);
		glm::mat4 circleLocalToWorld = glm::translate(glm::mat4(1), { position.x, position.y, 0 });
		glm::mat4 circleWorldToLocal = glm::inverse(circleLocalToWorld);

		pointInCircleSpace = circleWorldToLocal * glm::vec4(point.x, point.y, 0, 1);

		return b2PointInCircle({ pointInCircleSpace.x, pointInCircleSpace.y }, &circle);
	}
	else if (physics->shapeType == b2ShapeType::b2_polygonShape) {
		b2Polygon aabb = b2Shape_GetPolygon(physics->GetShapeId());
		glm::vec2 position = physics->GetPosition();

		glm::vec3 pointInAABBSpace = glm::vec3(point.x, point.y, 0);
		glm::mat4 aabbLocalToWorld = glm::translate(glm::mat4(1), { position.x, position.y, 0 });
		glm::mat4 aabbWorldToLocal = glm::inverse(aabbLocalToWorld);

		pointInAABBSpace = aabbWorldToLocal * glm::vec4(point.x, point.y, 0, 1);

		return b2PointInPolygon({ pointInAABBSpace.x, pointInAABBSpace.y },&aabb);
	}
}

void PhysicsProp::ShowUI()
{
	if (popupShowing) {
		//ImGui::Begin("Test menu", &popupShowing);
		ImGui::OpenPopup(name.c_str());

		if (ImGui::IsAnyMouseDown()) {
			popupShowing = false;
		}
		//ImGui::SetNextWindowPos(ImGui::GetMousePos());
	}

	if (ImGui::BeginPopup(name.c_str())) {

		glm::vec2 pos = physics->GetPosition();
		ImGui::Text(std::string("Position: " + std::to_string(pos.x) + ", " + std::to_string(pos.y)).c_str());

		b2AABB aabb = b2Shape_GetAABB(physics->GetShapeId());
		b2Vec2 aabbCentre = b2AABB_Center(aabb);
		b2Vec2 aabbsExtents = b2AABB_Extents(aabb);
		ImGui::Text(std::string("Half extents: " + std::to_string(aabbsExtents.x) + ", " + std::to_string(aabbsExtents.y)).c_str());

		ImGui::EndPopup();
	}
}

void PhysicsProp::Draw()
{
	Renderer::Instance->DrawGizmo(*physics, glm::vec3(1.0f, 0.0f, 0.0f));
}
