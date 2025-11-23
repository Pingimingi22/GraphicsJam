#pragma once
#include "Sprite.h"
#include <string>
#include "glm/mat3x3.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
#include "SpriteAnimated.h"
#include "PhysicsBody.h"
#include "glm/common.hpp"

class Renderer
{
private:
	unsigned int VBO;
	unsigned int VAO;

	unsigned int _lineRenderVBO;
	unsigned int _lineRenderVAO;

	unsigned int _circleRenderVBO;
	unsigned int _circleRenderVAO;

	unsigned int _pointToPointRenderVBO;
	unsigned int _pointToPointRenderVAO;

	unsigned int _shadowcastQuadRenderVBO;
	unsigned int _shadowcastQuadRenderVAO;

	unsigned int _drawCallCounter = 0;

public:
	static Renderer* Instance;

	class Camera
	{
	public:
		glm::vec3 position;
		float rotation;

		glm::mat4 GetViewMatrix();
		glm::mat4 projectionMatrix = glm::mat4(1.0f);
	};
	//class Camera;

	Camera camera;

	void Init(const Window& window);

	void Draw(Sprite sprite, bool isHighlighted=false);
	void Draw(SpriteAnimated sprite, float deltaTime);
	void DrawGizmo(PhysicsBody body, glm::vec3 colour);
	void DrawRay(b2RayCastInput ray, glm::vec3 colour, float lineWidth=2.5f);
	void DrawShadowcastQuad(std::vector<glm::vec3> vertices);
	void SetCameraPos(float x, float y);
	void DrawCircle(glm::vec2 position, float radius, glm::vec3 colour);

	unsigned int GetDrawCallsThisFrame();
	void Flush();

private:
	std::vector<glm::vec3> _unitCircleVerts;
};

