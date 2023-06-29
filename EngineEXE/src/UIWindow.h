#pragma once
#include "BasicIncludes.h"
#include "MouseEvent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class UIWindow
{
public:
	UIWindow(std::string name);
	~UIWindow() {}

	virtual void Render() {}
	virtual void HandleKeyboardInput(KeyEvent key) {}
	virtual void HandleMouseInput(MouseEvent e) {}

	inline void SetPosition(glm::vec2 pos) { m_position = pos; }
	inline glm::vec2 GetPosition() { return m_position; }

	inline void SetSize(glm::vec2 size) { m_size = size; }
	inline glm::vec2 GetSize() { return m_size; }

	inline void SetIsFocused(bool isFocused) { m_isFocused = isFocused; }
	inline bool GetIsFocused() { return m_isFocused; }

	inline void SetIsHovered(bool isHovered) { m_isHovered = isHovered; }
	inline bool GetIsHovered() { return m_isHovered; }

protected:
	std::string m_name;
	glm::vec2 m_position;
	glm::vec2 m_size;
	bool m_isFocused : 1;
	bool m_isHovered : 1;
};
