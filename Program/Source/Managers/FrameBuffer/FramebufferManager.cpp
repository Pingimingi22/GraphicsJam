#include "FramebufferManager.h"
#include "Application.h"

void FramebufferManager::Init() {
	CreateFramebuffers();
}

void FramebufferManager::CreateFramebuffers() {
	
	int width = Application::Instance->GetWindowWidth();
	int height = Application::Instance->GetWindowHeight();

	Framebuffer* _basic = Framebuffer::Create(
		Framebuffers::Basic,
		glm::vec2(width, height));

	Framebuffer* _shadows = Framebuffer::Create(
		Framebuffers::Shadows,
		glm::vec2(width, height));

	_buffers[Framebuffers::Basic] = _basic;
	_buffers[Framebuffers::Shadows] = _shadows;
}

Framebuffer* FramebufferManager::GetBuffer(
	std::string bufferName) {

	return _buffers[bufferName];
}

void FramebufferManager::Bind(std::string bufferName) {
	
	Framebuffer* buffer = GetBuffer(bufferName);
	buffer->Bind();
}

unsigned int FramebufferManager::GetFramebufferTexture(
	std::string bufferName) {
	
	Framebuffer* buffer = GetBuffer(bufferName);
	return buffer->Texture();
}