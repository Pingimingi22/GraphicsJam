#include "Player.h"


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

	b2ContactEvents contactEvents = b2World_GetContactEvents(GetPhysicsBody().GetWorldId());
	for (int i = 0; i < contactEvents.beginCount; ++i)
	{
		b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;
		std::cout << "Collision detected!" << std::endl;
		_isJumping = false;
	}
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
