#pragma once
#include "SpriteAnimated.h"
#include "PhysicsBody.h"
#include "GLFW/glfw3.h"
#include "Window.h"
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(Sprite* sprite, PhysicsBody physicsBody);
	~Player();

	void Update(float deltaTime, Window* window);
	void HandleInput(float deltaTime, Window* window);

	Sprite* GetSprite();
	SpriteAnimated* GetAnimatedSprite();
	PhysicsBody GetPhysicsBody();
	b2BodyId GetBodyId();

	float _moveSpeed = 5.0f;
	float _jumpStrength = 50.0f;

	void ShowTooltip() override;
	void ShowUI() override;
	bool IsPointOverlapping(glm::vec2 point) override;

private:
	Sprite* _sprite = nullptr;
	PhysicsBody _physics;

	bool _isJumping = false;
};

