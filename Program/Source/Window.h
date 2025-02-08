#pragma once
#include <string>

struct GLFWwindow;

class Window
{
public:
	Window(const std::string& title, uint32_t width, uint32_t height);
	~Window();

	void Shutdown();
	void ProcessEvents();
	void Clear(float r, float g, float b);
	void Clear(float r, float g, float b, float a);
	void SwapBuffers();

	bool ShouldClose();
	bool Initialised() const { return m_Initialised; }

	uint32_t Width() const;
	uint32_t Height() const;

	GLFWwindow* m_NativeWindow;
private:
	uint32_t m_Width, m_Height;
	std::string m_Title;
	bool m_Initialised = false;
};