#include "Player.h"
#include "Renderer.h"
#include "imgui.h"
#include <string>

Player::Player(Sprite* sprite, PhysicsBody physicsBody)
	: _sprite(sprite), _physics(physicsBody) {
	
	_sprite->SetScale(20.0f);
}

Player::~Player() 
{
	delete _sprite;
}

void Player::Update(float deltaTime, Window* window)
{
	HandleInput(deltaTime, window);

	glm::vec2 physicsPosition = _physics.GetPosition();
	_sprite->SetPosition(physicsPosition);
	Renderer::Instance->SetCameraPos(physicsPosition.x, physicsPosition.y);
}

void Player::HandleInput(float deltaTime, Window* window)
{
	float horizontalInput = 0;
	float verticalInput = 0;
	if (glfwGetKey(window->m_NativeWindow, GLFW_KEY_D) == GLFW_PRESS) {
		horizontalInput = 1;
	}
	if (glfwGetKey(window->m_NativeWindow, GLFW_KEY_A) == GLFW_PRESS) {
		horizontalInput = -1;
	}
	if (glfwGetKey(window->m_NativeWindow, GLFW_KEY_SPACE) == GLFW_PRESS &&
		!_isJumping) {
		verticalInput = 1;
		_isJumping = true;
		std::cout << "Jumping!!!" << std::endl;
	}

	// Flip sprite based on input.
	if (horizontalInput < 0) {
		GetSprite()->SetScale(glm::vec2(-20, 20));
	}
	else if (horizontalInput > 0) {
		GetSprite()->SetScale(glm::vec2(20, 20));
	}

	// Animate through flipbook shader.
	GetAnimatedSprite()->animationTimer += deltaTime;
	if (GetAnimatedSprite()->animationTimer >= 0.1f) {
		GetAnimatedSprite()->animationTimer = 0;

		SpriteAnimated* animatedSprite = GetAnimatedSprite();
		if (horizontalInput != 0)
		{
			int newIndex = animatedSprite->GetCurrentFrameIndex() + 1;
			animatedSprite->SetCurrentFrameIndex(newIndex);
		}
		else {
			animatedSprite->SetCurrentFrameIndex(0);
		}
	}

	glm::vec2 up = glm::vec2(0.0f, 1.0f);
	glm::vec2 right = glm::vec2(1.0f, 0.0f);
	glm::vec2 desiredVelocity = glm::vec2(right * horizontalInput) * _moveSpeed;

	b2Vec2 currentVelocity = b2Body_GetLinearVelocity(GetBodyId());
	glm::vec2 currentVelocityGlm = glm::vec2(currentVelocity.x, currentVelocity.y);

	glm::vec2 requiredForce = (desiredVelocity - currentVelocityGlm);
	
	b2Vec2 requiredForceB2D = b2Vec2();
	requiredForceB2D.x = requiredForce.x;
	requiredForceB2D.y = 0;

	b2Vec2 forcePoint = b2Vec2_zero;

	b2Vec2 jumpForce = b2Vec2();
	jumpForce.y = (up * _jumpStrength * verticalInput).y;
	b2Body_ApplyLinearImpulse(GetBodyId(), requiredForceB2D + jumpForce, forcePoint, true);


	glm::vec2 currentPosGlm = _physics.GetPosition();
	b2Vec2 currentPos = { currentPosGlm.x, currentPosGlm.y };
	b2QueryFilter filter = b2DefaultQueryFilter();

	b2Circle circleShapeCast = b2Circle();
	circleShapeCast.radius = 0.1f;
	b2Transform playerTransform = b2Body_GetTransform(_physics.GetId());

	struct idk {
		b2Vec2 jumpForce;
		bool* isJumping;
	} testStruct ;
	testStruct.jumpForce = jumpForce;
	testStruct.isJumping = &_isJumping;

	auto castCallback = [](b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
		idk* theStructReturns = (idk*)context;

		if (theStructReturns->jumpForce.y <= 0) {
			*(theStructReturns->isJumping) = false;
		}
	
		// Will return closest hit.
		return fraction;
	};
	b2TreeStats castResult = b2World_CastCircle(_physics.GetWorldId(), &circleShapeCast, playerTransform, { 0.0, -1.0f }, filter, castCallback, &testStruct);
	

	/*
	b2ContactEvents contactEvents = b2World_GetContactEvents(GetPhysicsBody().GetWorldId());
	for (int i = 0; i < contactEvents.beginCount; ++i)
	{
		b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;
		std::cout << "Collision detected!" << std::endl;
		_isJumping = false;
	}*/
}

Sprite* Player::GetSprite()
{
	return _sprite;
}

SpriteAnimated* Player::GetAnimatedSprite()
{
	return (SpriteAnimated*)GetSprite();
}

PhysicsBody Player::GetPhysicsBody()
{
	return _physics;
}

b2BodyId Player::GetBodyId()
{
	return GetPhysicsBody().GetId();
}

void Player::ShowTooltip()
{
	ImGui::BeginTooltip();
	ImGui::Text("Player");
	ImGui::EndTooltip();
}

void Player::ShowUI()
{
	ImGui::Begin("Player");
	ImGui::Text(std::string("Is Jumping: " + std::to_string(_isJumping)).c_str());
	ImGui::End();
}

bool Player::IsPointOverlapping(glm::vec2 point)
{
	b2Circle circle = b2Shape_GetCircle(_physics.GetShapeId());
	glm::vec2 position = _physics.GetPosition();

    glm::vec3 pointInCircleSpace = glm::vec3(point.x, point.y, 0);
	glm::mat4 circleLocalToWorld = glm::translate(glm::mat4(1), { position.x, position.y, 0 });
	glm::mat4 circleWorldToLocal = glm::inverse(circleLocalToWorld);
	
	pointInCircleSpace = circleWorldToLocal * glm::vec4(point.x, point.y, 0, 1);

	return b2PointInCircle({ pointInCircleSpace.x, pointInCircleSpace.y}, &circle);
}
