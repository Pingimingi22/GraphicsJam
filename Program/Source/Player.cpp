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
	b2Vec2 force = b2Vec2();

	if (glfwGetKey(window->m_NativeWindow, GLFW_KEY_W) == GLFW_PRESS) {
		force.y += 5;
	}
	if (glfwGetKey(window->m_NativeWindow, GLFW_KEY_S) == GLFW_PRESS) {
		force.y -= 5;
	}
	if (glfwGetKey(window->m_NativeWindow, GLFW_KEY_D) == GLFW_PRESS) {
		force.x += 5;
	}
	if (glfwGetKey(window->m_NativeWindow, GLFW_KEY_A) == GLFW_PRESS) {
		force.x -= 5;
	}

	if (force.x < 0) {
		GetSprite()->SetScale(glm::vec2(-20, 20));
	}
	else if (force.x > 0) {
		GetSprite()->SetScale(glm::vec2(20, 20));
	}

	GetAnimatedSprite()->animationTimer += deltaTime;
	if (GetAnimatedSprite()->animationTimer >= 0.1f) {
		GetAnimatedSprite()->animationTimer = 0;

		SpriteAnimated* animatedSprite = GetAnimatedSprite();
		if (force.x != 0)
		{
			int newIndex = animatedSprite->GetCurrentFrameIndex() + 1;
			animatedSprite->SetCurrentFrameIndex(newIndex);
		}
		else {
			animatedSprite->SetCurrentFrameIndex(0);
		}
	}

	b2Vec2 forcePoint = b2Vec2_zero;
	b2Body_ApplyForce(
		GetBodyId(),
		force,
		forcePoint,
		true);
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
