#pragma once
#include "box2d/box2d.h"
#include "glm/vec2.hpp"

class PhysicsBody
{
public:
	PhysicsBody( 
		b2WorldId world, 
		b2BodyType type, 
		glm::vec2 initialPos);

	b2BodyId GetId();

	void ApplyForce(glm::vec2 force);
	void ChangeBodyType(b2BodyType type);
	
	glm::vec2 GetLinearVelocity();
	glm::vec2 GetPosition();

	void DrawGizmo(b2HexColor colour);

private:
	b2BodyId _id;
	b2WorldId _worldId;
};

