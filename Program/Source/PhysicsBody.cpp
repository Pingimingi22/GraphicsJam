#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(
	b2WorldId world, 
	b2BodyType type,
	glm::vec2 initialPos)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = type;

	b2Vec2 bodyPosition;
	bodyPosition.x = initialPos.x;
	bodyPosition.y = initialPos.y;
	bodyDef.position = bodyPosition;

	// Freeze rotation
	bodyDef.fixedRotation = true;

	// Test shapes. Will change later.
	b2Polygon dynamicBox = b2MakeRoundedBox(0.5f, 0.5f, 0.25f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.1f;

	_id = b2CreateBody(world, &bodyDef);
	_worldId = world;

	b2CreatePolygonShape(_id, &shapeDef, &dynamicBox);
}

b2BodyId PhysicsBody::GetId()
{
	return _id;
}

glm::vec2 PhysicsBody::GetPosition()
{
	b2Vec2 physicsPos = b2Body_GetPosition(_id);
	glm::vec2 glmPos = glm::vec2(physicsPos.x, physicsPos.y);
	return glmPos;
}
