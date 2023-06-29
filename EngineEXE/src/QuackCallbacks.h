#pragma once
#include "BasicIncludes.h"

namespace QuackEngine {

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	void window_size_callback(GLFWwindow* window, int width, int height);
	void window_close_callback(GLFWwindow* window);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
} // quack engine namespace

