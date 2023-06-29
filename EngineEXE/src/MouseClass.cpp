#include "pch.h"

#include "MouseClass.h"

//#include <directxmath.h>
//#include <d3d11_1.h>

//using namespace DirectX;

std::queue<MouseEvent> MouseClass::s_EventBuffer;
bool MouseClass::s_IsLeftClick;
bool MouseClass::s_IsRightClick;
bool MouseClass::s_IsScrollClick;
Vertex2 MouseClass::s_Position;
Vertex2 MouseClass::s_Distance;
float MouseClass::s_DiffX;
float MouseClass::s_DiffY;

void MouseClass::Init()
{
	s_IsLeftClick = false;
	s_IsRightClick = false;
	s_IsScrollClick = false;

	s_Position = { 0, 0 };
	s_Distance = { 0, 0 };

	s_DiffX = 0;
	s_DiffY = 0;
}

void MouseClass::OnLeftClick(int x, int y)
{
	s_IsLeftClick = true;
	MouseEvent me(MouseEvent::EventType::L_CLICK, x, y);
	s_EventBuffer.push(me);
}

void MouseClass::OnLeftRelease(int x, int y)
{
	s_IsLeftClick = false;
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::L_RELEASE, x, y));
}

void MouseClass::OnLeftHeld(int x, int y)
{
	s_IsLeftClick = true;
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::L_HELD, x, y));
}

void MouseClass::OnRightClick(int x, int y)
{
	s_IsRightClick = true;
	MouseEvent me(MouseEvent::EventType::R_CLICK, x, y);
	s_EventBuffer.push(me);
}

void MouseClass::OnRightRelease(int x, int y)
{
	s_IsRightClick = false;
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::R_RELEASE, x, y));
}

void MouseClass::OnRightHeld(int x, int y)
{
	s_IsRightClick = true;
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::R_HELD, x, y));
}

void MouseClass::OnScrollClick(int x, int y)
{
	s_IsScrollClick = true;
	MouseEvent me(MouseEvent::EventType::SCROLL_CLICK, x, y);
	s_EventBuffer.push(me);
}

void MouseClass::OnScrollRelease(int x, int y)
{
	s_IsScrollClick = false;
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::SCROLL_RELEASE, x, y));
}

void MouseClass::OnScrollHeld(int x, int y)
{
	s_IsScrollClick = true;
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::SCROLL_HELD, x, y));
}

void MouseClass::OnScrollUp(int x, int y)
{
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::SCROLL_UP, x, y));
}

void MouseClass::OnScrollDown(int x, int y)
{
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::SCROLL_DOWN, x, y));
}

void MouseClass::OnMouseMove(int x, int y)
{
	s_DiffX = glm::radians(0.25f*static_cast<float>(x-s_Position.x));
	s_DiffY = glm::radians(0.25f*static_cast<float>(y-s_Position.y));

	s_Position.x = x;
	s_Position.y = y;
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::MOVE, s_Position));
}

void MouseClass::OnMouseMoveRaw(int x, int y)
{
	s_EventBuffer.push(MouseEvent(MouseEvent::EventType::MOVE_RAW, x, y));
}

MouseEvent MouseClass::ReadEvent()
{
	if(s_EventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e  = s_EventBuffer.front(); // Get the first event in the queue
		s_EventBuffer.pop(); // remove the first event from the buffer
		return e;
	}
}