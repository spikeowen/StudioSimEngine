#include "pch.h"

#include "Window.h"
#include "Quack.h"

Window::Window(std::string name, int width, int height, FullScreenMode fullScreenMode = FullScreenMode::WINDOWED) :
	m_name(name),
	m_width(width),
	m_height(height),
	m_fullScreenMode(fullScreenMode)
{
}

bool Window::UseWindow()
{
	// Check if glfw has been initialized before
	if (!Quack::s_glfwInitialised)
	{
		if (!glfwInit())
		{
			Quack::s_glfwInitialised = false;
			return false;
		}
		else
		{
			Quack::s_glfwInitialised = true;
			m_primaryMonitor = glfwGetPrimaryMonitor();
		}
	}

	switch (m_fullScreenMode)
	{
	case FullScreenMode::WINDOWED:
		m_GLFWwindow = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
		break;
	case FullScreenMode::BORDERLESS_FULL_SCREEN:
		m_vidMode = glfwGetVideoMode(m_primaryMonitor);
		glfwWindowHint(GLFW_RED_BITS, m_vidMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, m_vidMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, m_vidMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, m_vidMode->refreshRate);

		m_GLFWwindow = glfwCreateWindow(m_vidMode->width, m_vidMode->height, m_name.c_str(), m_primaryMonitor, NULL);
		break;
	case FullScreenMode::FULL_SCREEN:
		m_GLFWwindow = glfwCreateWindow(m_width, m_height, m_name.c_str(), m_primaryMonitor, NULL);
		break;
	}

	if (!m_GLFWwindow)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_GLFWwindow);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	return true;
}

Window::~Window()
{

}


