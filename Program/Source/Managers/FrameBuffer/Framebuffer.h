#pragma once
#include "glm/vec2.hpp"
#include <string>

class Framebuffer {
public:
	Framebuffer(
		std::string name,
		unsigned int fbo,
		unsigned int rbo,
		unsigned int texture);

	static Framebuffer* Create(
		std::string name, 
		glm::vec2 dimensions);

	void Bind();
	unsigned int Texture();

private:
	std::string _name;
	unsigned int _fbo;
	unsigned int _rbo;
	unsigned int _texture;
};