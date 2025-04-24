#include "CircleProp.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include "Application.h"
#include "Renderer.h"

CircleProp::CircleProp(std::string name, float radius, glm::vec2 position)
{
	_name = name;
	PhysicsBody* newCircleBody = new PhysicsBody(
		Application::Instance->WorldId,
		b2_dynamicBody,
		position,
		radius,
		false);
	_physics = newCircleBody;
}

bool CircleProp::IsPointOverlapping(glm::vec2 point)
{
	b2Circle circle = b2Shape_GetCircle(_physics->GetShapeId());
	glm::vec2 position = _physics->GetPosition();

	glm::vec3 pointInCircleSpace = glm::vec3(point.x, point.y, 0);
	glm::mat4 circleLocalToWorld = glm::translate(glm::mat4(1), { position.x, position.y, 0 });
	glm::mat4 circleWorldToLocal = glm::inverse(circleLocalToWorld);

	pointInCircleSpace = circleWorldToLocal * glm::vec4(point.x, point.y, 0, 1);

	return b2PointInCircle({ pointInCircleSpace.x, pointInCircleSpace.y }, &circle);
}

void CircleProp::ShowUI()
{
}

void CircleProp::Draw()
{
	Renderer::Instance->DrawGizmo(*_physics, glm::vec3(1.0f, 0.0f, 0.0f));
}

void CircleProp::DrawShadow(glm::vec2 playerPosition)
{

}

void CircleProp::ShowTooltip()
{
	ImGui::BeginTooltip();
	ImGui::Text(std::string(_name + " physics prop").c_str());
	if (ImGui::IsMouseClicked(1)) {
		_popupShowing = true;
	}
	ImGui::EndTooltip();
}
