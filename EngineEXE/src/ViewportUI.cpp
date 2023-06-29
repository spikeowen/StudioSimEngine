#include "pch.h"

#include "ViewportUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Quack.h"
#include "OrthographicCamera.h"
#include "MouseClass.h"

ViewportUI::ViewportUI(std::string name) : UIWindow(name)
{
	startViewportX = 0;
	startViewportY = 0;
}

ViewportUI::~ViewportUI()
{

}

void ViewportUI::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(m_name.c_str());
	m_isHovered = ImGui::IsWindowHovered();

	// Check if the size of the window changed
	// *((glm::vec2*)& is used to compare imvec2 and glm vec2 and it works due to their layouts both being two floats
	ImVec2 viewportPos = ImGui::GetWindowPos();
	if (m_position != *((glm::vec2*)&viewportPos))
	{
		m_position = { viewportPos.x, viewportPos.y };
	}

	// Check if the size of the window changed
	// *((glm::vec2*)& is used to compare imvec2 and glm vec2 and it works due to their layouts both being two floats
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	if (m_size != *((glm::vec2*)&viewportSize))
	{
		m_size = { viewportSize.x, viewportSize.y };
		Quack::GetFrameBuffer()->Resize(m_size.x, m_size.y);

		float aspect = m_size.x / m_size.y;

		if (Quack::GetOrthoCam())
		{
			Quack::GetOrthoCam()->RecalculateProjection(-Quack::GetOrthoCam()->GetZoom() * aspect, Quack::GetOrthoCam()->GetZoom() * aspect, -Quack::GetOrthoCam()->GetZoom(), Quack::GetOrthoCam()->GetZoom());
		}

	}

	startViewportX = ImGui::GetCursorScreenPos().x;
	startViewportY = ImGui::GetCursorScreenPos().y;
	m_isFocused = ImGui::IsWindowFocused();
	ImGui::Image((void*)Quack::GetFrameBuffer()->GetID(), ImVec2(m_size.x, m_size.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void ViewportUI::HandleKeyboardInput(KeyEvent key)
{
	if (key.IsHeld() || key.IsPressed())
	{
		switch (key.GetKeyCode())
		{
		case QE_KEY_LEFT_ARROW:
			m_newPosition = Quack::GetOrthoCam()->GetPosition() - glm::vec3(Quack::GetOrthoCam()->GetMoveSpeed(), 0.0f, 0.0f);
			Quack::GetOrthoCam()->SetPosition(m_newPosition);
			break;


		case QE_KEY_RIGHT_ARROW:
			m_newPosition = Quack::GetOrthoCam()->GetPosition() + glm::vec3(Quack::GetOrthoCam()->GetMoveSpeed(), 0.0f, 0.0f);
			Quack::GetOrthoCam()->SetPosition(m_newPosition);
			break;


		case QE_KEY_UP_ARROW:
			m_newPosition = Quack::GetOrthoCam()->GetPosition() + glm::vec3(0.0f, Quack::GetOrthoCam()->GetMoveSpeed(), 0.0f);
			Quack::GetOrthoCam()->SetPosition(m_newPosition);
			break;

		case QE_KEY_DOWN_ARROW:
			m_newPosition = Quack::GetOrthoCam()->GetPosition() - glm::vec3(0.0f, Quack::GetOrthoCam()->GetMoveSpeed(), 0.0f);
			Quack::GetOrthoCam()->SetPosition(m_newPosition);
			break;

		default:
			break;
		}
	}
}

void ViewportUI::HandleMouseInput(MouseEvent e)
{
	if (Quack::GetOrthoCam())
	{
		if (Quack::GetOrthoCam()->GetCanZoom())
		{
			//Spikes child
			//Vector2 port;
			////Size of viewport
			//port.x = GetSize().x;
			//port.y = GetSize().y;

			////Current mouse position within viewport scale
			//Vector2 viewStart;
			//viewStart.x = ImGui::GetMousePos().x - GetStartX();
			//viewStart.y = ImGui::GetMousePos().y - GetStartY();

			//Only counting the click within viewport boundary
			if (m_isHovered)
			{
				float newZoom = Quack::GetOrthoCam()->GetZoom();
				float aspect = m_size.x / m_size.y;

				if (e.GetType() == MouseEvent::EventType::SCROLL_DOWN)
				{
					newZoom += (float)e.GetType() * Quack::GetOrthoCam()->GetZoomSpeed();
				}

				if (e.GetType() == MouseEvent::EventType::SCROLL_UP)
				{
					newZoom -= (float)e.GetType() * Quack::GetOrthoCam()->GetZoomSpeed();
				}

				newZoom = newZoom <= 1.0f ? 1.0f : newZoom;
				Quack::GetOrthoCam()->SetZoom(newZoom, aspect);
			}
		}

	}

}
