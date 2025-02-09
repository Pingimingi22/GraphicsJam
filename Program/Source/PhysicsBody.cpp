#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(
	b2WorldId world, 
	b2BodyType type,
	glm::vec2 initialPos,
	float halfWidth,
	float halfHeight)
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
	b2Polygon dynamicBox = b2MakeRoundedBox(halfWidth, halfHeight, 0.25f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();

	shapeDef.enableContactEvents = true;
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.0f;

	_bodyId = b2CreateBody(world, &bodyDef);
	_worldId = world;

	_shapeId = b2CreatePolygonShape(_bodyId, &shapeDef, &dynamicBox);
}

b2BodyId PhysicsBody::GetId()
{
	return _bodyId;
}

b2ShapeId PhysicsBody::GetShapeId()
{
	return _shapeId;
}

b2WorldId PhysicsBody::GetWorldId()
{
	return _worldId;
}

void PhysicsBody::SetPosition(glm::vec2 position)
{
	b2Vec2 newPos = b2Vec2();
	newPos.x = position.x;
	newPos.y = position.y;
	b2Rot newRot = b2Rot();
	newRot.c = 1;
	newRot.s = 0;
	b2Body_SetTransform(_bodyId, newPos, newRot);
}

glm::vec2 PhysicsBody::GetPosition()
{
	b2Vec2 physicsPos = b2Body_GetPosition(_bodyId);
	glm::vec2 glmPos = glm::vec2(physicsPos.x, physicsPos.y);
	return glmPos;
}
