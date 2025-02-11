#include "Window.h"

#include <iostream>

#define GLFW_INCLUDE_NONE
#define GLAD_GL_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <glad/gl.h>

static bool s_GLFWInitialised = false;

Window* Window::Instance = nullptr;

static void Log(const std::string& description)
{
	std::cout << description << std::endl;
}

static void GLFWErrorCallback(int32_t errorCode, const char* errorDescription)
{
	std::cout << "GLFW Error: (" << errorCode << ") - " << errorDescription << std::endl;
}

static void GPUInfoDump()
{
	std::string version = (const char*)glGetString(GL_VERSION);
	std::string vendor = (const char*)glGetString(GL_VENDOR);
	std::string renderer = (const char*)glGetString(GL_RENDERER);

	Log("OpenGL Information:");
	Log("\tVersion: " + version);
	Log("\tVendor:  " + vendor);
	Log("\tDevice:  " + renderer);
}

Window::Window(const std::string& title, uint32_t width, uint32_t height)
	: m_Title(title), m_Width(width), m_Height(height)
{
	if (Instance == nullptr) {
		Instance = this;
	}

	// Initialise glfw
	s_GLFWInitialised = glfwInit();

	if (!s_GLFWInitialised)
	{
		// Freak out
		Log("Failed to initialise glfw");
		m_Initialised = false;
		return;
	}

	// Set gl context version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Set print callback for errors
	glfwSetErrorCallback(GLFWErrorCallback);

	// Create window
	m_NativeWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (!m_NativeWindow)
	{
		// Freak out
		Log("Failed to create window");
		m_Initialised = false;
		return;
	}

	// Set created window as current context
	glfwMakeContextCurrent(m_NativeWindow);

	// Initialise glad
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		// Freak out
		Log("Failed to initialise glad");
		m_Initialised = false;
		return;
	}

	// Success
	m_Initialised = true;

	Log("Window successfully created");
	GPUInfoDump();
}

Window::~Window()
{
	Shutdown();
}

void Window::Shutdown()
{
	if (s_GLFWInitialised)
	{
		// Be aware that terminate should not be called until the
		// application is shutting down (i.e. glfw is not longer of use)
		// as *all* resources allocated by the glfw library will be freed
		// as a result and any classes that still rely on glfw functions
		// will throw a read access violation error.

		// This setup assumes that only one window will ever be created
		// at a time and that the destruction of that window assumes the
		// application is shutting down.

		glfwDestroyWindow(m_NativeWindow);
		glfwTerminate();
		m_Initialised = false;
	}
}

void Window::ProcessEvents()
{
	// Processes all pending events relating to glfw
	// (mouse, keyboard, window, etc)
	glfwPollEvents();
}

void Window::Clear(float r, float g, float b)
{
	Clear(r, g, b, 1.0f);
}

void Window::Clear(float r, float g, float b, float a)
{
	// Sets the "background" colour of the
	// framebuffer being rendered to
	glClearColor(r, g, b, a);

	// Clears the framebuffer being rendered to
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::SwapBuffers()
{
	// Swaps the framebuffer currently presented with
	// the framebuffer that was just rendered to
	glfwSwapBuffers(m_NativeWindow);
}

bool Window::ShouldClose()
{
	// Detects when the user clicks on the X button
	return glfwWindowShouldClose(m_NativeWindow);
}

uint32_t Window::Width() const
{
	return m_Width;
}

uint32_t Window::Height() const
{
	return m_Height;
}
