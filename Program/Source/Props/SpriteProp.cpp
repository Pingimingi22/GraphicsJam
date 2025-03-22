#include "SpriteProp.h"
#include "Renderer.h"
#include "Application.h"
#include "InputManager.h"

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
