#include "pch.h"

#include "QuackCallbacks.h"
#include "KeyboardClass.h"
#include "MouseClass.h"
#include "Quack.h"
#include "UILayer.h"

namespace QuackEngine {
	// Input callback
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (action)
		{
		case GLFW_PRESS:
			KeyboardClass::OnKeyPressed(key);
			KeyboardClass::OnKeyHeld(key);
			break;
		case GLFW_RELEASE:
			KeyboardClass::OnKeyReleased(key);
			break;
		}
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			double posX, posY;
			glfwGetCursorPos(window, &posX, &posY);
			if (action == GLFW_PRESS)
			{
				MouseClass::OnRightClick(posX, posY);
			}
			else if (action == GLFW_RELEASE)
			{
				MouseClass::OnRightRelease(posX, posY);
			}
			else if (action == GLFW_REPEAT)
			{
				MouseClass::OnRightHeld(posX, posY);
			}
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			double posX, posY;
			glfwGetCursorPos(window, &posX, &posY);
			if (action == GLFW_PRESS)
			{
				MouseClass::OnLeftClick(posX, posY);
			}
			else if (action != GLFW_RELEASE)
			{
				MouseClass::OnLeftRelease(posX, posY);
			}
			else if (action == GLFW_REPEAT)
			{
				MouseClass::OnLeftHeld(posX, posY);
			}
		}
	}

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		MouseClass::OnMouseMove(xpos, ypos);
	}

	// Window resize callback
	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}


	// window close callback
	void window_close_callback(GLFWwindow* window)
	{
		Quack::s_running = false;
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		//QE_LOG(xoffset);
		//QE_LOG(yoffset);

		if (yoffset > 0)
			MouseClass::OnScrollUp(xoffset, yoffset);
		else if (yoffset < 0)
			MouseClass::OnScrollDown(xoffset, yoffset);
	}
}