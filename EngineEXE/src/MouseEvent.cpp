#include "pch.h"

#include "MouseEvent.h"

#define M_PI           3.14159265358979323846  /* pi */

MouseEvent::MouseEvent() 
	: m_Position(0,0), m_Distance(0,0), m_Type(EventType::INVALID)
{
}

MouseEvent::MouseEvent(const EventType type, Vertex2 position)
	: m_Position(position), m_Distance(0,0), m_Type(type)
{
}

MouseEvent::MouseEvent(const EventType type, const int x, const int y) 
	: m_Position(x, y), m_Distance(0,0), m_Type(type)
{
}

MouseEvent::MouseEvent(const EventType type, Vertex2 position, Vertex2 distance)
	: m_Position(position), m_Distance(distance), m_Type(type)
{
}

MouseEvent::MouseEvent(const EventType type, int x, int y, int distanceX, int distanceY)
	: m_Position(x, y), m_Distance(distanceX, distanceY), m_Type(type)
{
}
