#pragma once
#include "Framebuffer.h"
#include "glad/gl.h"
#include <iostream>

Framebuffer::Framebuffer(
	std::string name,
	unsigned int fbo,
	unsigned int rbo,
	unsigned int texture) {

	_name = name;
	_fbo = fbo;
	_rbo = rbo;
	_texture = texture;
}

Framebuffer* Framebuffer::Create(
	std::string name, 
	glm::vec2 dimensions) {
	
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGB, 
		dimensions.x, 
		dimensions.y, 
		0, 
		GL_RGB, 
		GL_UNSIGNED_BYTE, 
		NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(
		GL_RENDERBUFFER, 
		GL_DEPTH24_STENCIL8, 
		dimensions.x,
		dimensions.y);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "victory dance" << std::endl;
	}

	return new Framebuffer(name, fbo, rbo, texture);
}

void Framebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _rbo);
}

unsigned int Framebuffer::Texture() {
	return _texture;
}