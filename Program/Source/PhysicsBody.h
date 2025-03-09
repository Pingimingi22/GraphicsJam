#pragma once
#include "box2d/box2d.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class PhysicsBody
{
public:
	PhysicsBody(
		b2WorldId world,
		b2BodyType type,
		glm::vec2 initialPos,
		float halfWidth,
		float halfHeight,
		bool freezeRotation = false);
	PhysicsBody(
		b2WorldId world,
		b2BodyType type,
		glm::vec2 initialPos,
		float radius,
		bool freezeRotation=false);

	b2ShapeType shapeType;

	b2BodyId GetId();
	b2ShapeId GetShapeId();
	b2WorldId GetWorldId();

	void ApplyForce(glm::vec2 force);
	void ChangeBodyType(b2BodyType type);
	
	void SetPosition(glm::vec2 position);

	void JoinObject(PhysicsBody otherBody, float jointLength);

	glm::vec2 GetPosition();
	float GetRotation();

	std::vector<b2JointId> _joints;
	bool HasJoints() { return _joints.size() > 0; }

	float halfWidth;
	float halfHeight;

private:
	b2BodyId _bodyId;
	b2WorldId _worldId;
	b2ShapeId _shapeId;

	void CreateSquareShape(float halfWidth, float halfHeight);
	void CreateCircleShape(float radius);
};

