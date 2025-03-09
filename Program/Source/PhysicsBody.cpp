#include "PhysicsBody.h"
#include "InputManager.h"
#include <iostream>

PhysicsBody::PhysicsBody(
	b2WorldId world, 
	b2BodyType type,
	glm::vec2 initialPos,
	float halfWidth,
	float halfHeight,
	bool freezeRotation)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = type;

	b2Vec2 bodyPosition;
	bodyPosition.x = initialPos.x;
	bodyPosition.y = initialPos.y;
	bodyDef.position = bodyPosition;

	// Freeze rotation
	bodyDef.fixedRotation = freezeRotation;

	_bodyId = b2CreateBody(world, &bodyDef);
	CreateSquareShape(halfWidth, halfHeight);
	_worldId = world;

}
PhysicsBody::PhysicsBody(
	b2WorldId world,
	b2BodyType type,
	glm::vec2 initialPos,
	float radius,
	bool freezeRotation)
{
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = type;

	b2Vec2 bodyPosition;
	bodyPosition.x = initialPos.x;
	bodyPosition.y = initialPos.y;
	bodyDef.position = bodyPosition;

	// Freeze rotation
	bodyDef.fixedRotation = freezeRotation;

	_bodyId = b2CreateBody(world, &bodyDef);
	CreateCircleShape(radius);
	_worldId = world;
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

b2JointId PhysicsBody::GetMouseJointId()
{
	return _mouseJointId;
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

void PhysicsBody::JoinObject(PhysicsBody otherBody, float jointLength)
{
	b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();
	jointDef.bodyIdA = _bodyId;
	jointDef.bodyIdB = otherBody._bodyId;
	jointDef.length = jointLength;
	jointDef.localAnchorA = {0.0f, 0.0f};
	jointDef.localAnchorB = { 0.0f, 0.0f };
	jointDef.maxLength = jointLength;
	jointDef.minLength = 0;
	
	_joints.push_back(b2CreateDistanceJoint(_worldId, &jointDef));
}

void PhysicsBody::JoinObjectToMouse()
{
	if (_mouseJointId.index1 == 0) {
		glm::vec3 mouseWorldPoint = InputManager::Instance->GetMouseWorldPoint();

		b2BodyDef groundBodyDef = b2DefaultBodyDef();
		groundBodyDef.type = b2_staticBody;
		//groundBodyDef.position = { mouseWorldPoint.x, mouseWorldPoint.y };
		b2BodyId groundBody = b2CreateBody(_worldId, &groundBodyDef);

		b2MouseJointDef mouseJoint = b2DefaultMouseJointDef();
		mouseJoint.bodyIdA = groundBody;
		mouseJoint.bodyIdB = _bodyId;
		mouseJoint.maxForce = 5000;
		mouseJoint.dampingRatio = 10.0f;
		mouseJoint.hertz = 10.0f;

		mouseJoint.target = { mouseWorldPoint.x, mouseWorldPoint.y };

		_mouseJointId = b2CreateMouseJoint(_worldId, &mouseJoint);

		InputManager::Instance->BodyJoinedToMouse = this;
	}
	else {
		std::cout << "Tried to create mouse joint but joint already exists." << std::endl;
	}
}

void PhysicsBody::BreakMouseJoint()
{
	b2DestroyJoint(_mouseJointId);
	_mouseJointId.index1 = 0;
}

glm::vec2 PhysicsBody::GetPosition()
{
	b2Vec2 physicsPos = b2Body_GetPosition(_bodyId);
	glm::vec2 glmPos = glm::vec2(physicsPos.x, physicsPos.y);
	return glmPos;
}

float PhysicsBody::GetRotation() {
	b2Rot rotation = b2Body_GetRotation(_bodyId);
	return atan2(rotation.s, rotation.c);
}

void PhysicsBody::CreateSquareShape(float halfWidth, float halfHeight)
{
	// Test shapes. Will change later.
	b2Polygon dynamicBox = b2MakeRoundedBox(halfWidth, halfHeight, 0.0f);

	b2ShapeDef shapeDef = b2DefaultShapeDef();

	shapeDef.enableContactEvents = true;
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.3f;

	_shapeId = b2CreatePolygonShape(_bodyId, &shapeDef, &dynamicBox);

	shapeType = b2ShapeType::b2_polygonShape;

	this->halfWidth = halfWidth;
	this->halfHeight = halfHeight;
}

void PhysicsBody::CreateCircleShape(float radius)
{
	b2Circle dynamicCircle;
	dynamicCircle.radius = radius;
	dynamicCircle.center = b2Vec2_zero;

	b2ShapeDef shapeDef = b2DefaultShapeDef();

	shapeDef.enableContactEvents = true;
	shapeDef.density = 1.0f;
	shapeDef.friction = 1.0f;

	_shapeId = b2CreateCircleShape(_bodyId, &shapeDef, &dynamicCircle);

	shapeType = b2ShapeType::b2_circleShape;
}
