#include "pch.h"

#include "UIWindow.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

UIWindow::UIWindow(std::string name) : m_name(name), m_position(0,0), m_size(0,0), m_isFocused(false)
{
}
