#pragma once
#include "glad/gl.h"
#include "glm/vec2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"
#include <iostream>

class Sprite {
private:

public:
	GLuint _texId;
	Sprite(std::string texturePath, glm::vec3 position);

	float rotation;
	glm::vec3 _position = glm::vec3(0.0f);
	glm::vec3 _scale = glm::vec3(1.0f);

	glm::mat4 ObjToWorld();
	glm::mat4 WorldToObj();
};