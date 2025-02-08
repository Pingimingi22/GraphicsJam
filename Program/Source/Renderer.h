#pragma once
#include "Sprite.h"
#include <string>
#include "glm/mat3x3.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"

class Renderer
{

private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int shaderProgram;

	std::string ReadShader(std::string path);

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

	
};

