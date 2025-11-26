#pragma once
#include "Framebuffer.h"
#include <map>

static class Framebuffers {
public:
	static const inline std::string Basic = "basic";
	static const inline std::string Shadows = "shadows";
	static const inline std::string Composite = "composite";
};

class FramebufferManager
{
public:
	FramebufferManager() {}
	void Init();

	void Bind(std::string bufferName);
	unsigned int GetFramebufferTexture(
		std::string bufferName);

private:
	void CreateFramebuffers();

	Framebuffer* GetBuffer(std::string bufferName);
	
	std::map<std::string, Framebuffer*> _buffers;
};


