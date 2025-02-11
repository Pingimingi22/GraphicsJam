#pragma once
#include "glad/gl.h"
#include "glm/vec2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"
#include <iostream>

class Sprite {
private:
	GLuint _texId;
	float rotation;
	glm::vec3 _position = glm::vec3(0.0f);
	glm::vec3 _scale = glm::vec3(1.0f);

public:
	Sprite(std::string texturePath,
		   glm::vec3 position,
		   glm::vec2 scale = glm::vec2(1.0f));

	GLuint GetTextureId();

	glm::vec2 GetPosition();
	void SetPosition(glm::vec2 newPosition);
	glm::vec2 GetRotation();
	void SetRotation(float newRotation);
	float GetScale();
	void SetScale(float uniformScale);
	void SetScale(glm::vec2 nonUniformScale);

	glm::mat4 ObjToWorld();
	glm::mat4 WorldToObj();
};