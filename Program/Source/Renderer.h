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

	unsigned int _basicShaderProgram;
	unsigned int _flipbookShaderProgram;
	unsigned int _lineShaderProgram;

	std::string ReadShader(std::string path);

	unsigned int _lineRenderVBO;
	unsigned int _lineRenderVAO;

	unsigned int _circleRenderVBO;
	unsigned int _circleRenderVAO;

public:
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

	void Draw(Sprite sprite, float deltaTime);
	void Draw(SpriteAnimated sprite, float deltaTime);
	void DrawGizmo(PhysicsBody body, glm::vec3 colour);

private:
	void ConfigureShader(unsigned int shaderProgram, unsigned int texture, glm::mat4 modelMat);

	unsigned int CreateVertexShader(std::string path);
	unsigned int CreateFragmentShader(std::string path);

	unsigned int CreateShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);

	std::vector<glm::vec3> _unitCircleVerts;
};

