#pragma once
#include "BasicIncludes.h"

enum class FullScreenMode
{
	FULL_SCREEN,
	BORDERLESS_FULL_SCREEN,
	WINDOWED
};

class Window
{
public:
	Window(std::string name, int width, int height, FullScreenMode fullScreenMode);
	~Window();

	bool UseWindow();
	inline int GetHeight() const { return m_height; }
	inline int GetWidth() const { return m_width; }

	inline GLFWwindow* GetGLFWWindow() const { return m_GLFWwindow; }
private:
	GLFWwindow* m_GLFWwindow;
	std::string m_name;
	int m_width;
	int m_height;
	FullScreenMode m_fullScreenMode;
	GLFWmonitor* m_primaryMonitor;
	const GLFWvidmode* m_vidMode;
};

