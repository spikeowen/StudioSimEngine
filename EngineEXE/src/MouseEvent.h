#pragma once

struct Vertex2
{
	int x, y;

	Vertex2() : x(0), y(0) {}

	Vertex2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

class MouseEvent
{
public:
	enum class EventType
	{
		L_CLICK,
		L_HELD,
		L_RELEASE,
		R_CLICK,
		R_HELD,
		R_RELEASE,
		SCROLL_CLICK,
		SCROLL_HELD,
		SCROLL_RELEASE,
		SCROLL_UP,
		SCROLL_DOWN,
		MOVE,
		MOVE_RAW,
		INVALID	
	};
private:
	EventType m_Type;
	Vertex2 m_Position, m_Distance;
public:
	MouseEvent();
	MouseEvent(const EventType type, Vertex2 position);
	MouseEvent(const EventType type, int x, int y);
	MouseEvent(const EventType type, Vertex2 position, Vertex2 Distance);
	MouseEvent(const EventType type, int x, int y, int distanceX, int distanceY);
	inline bool IsValid() const { return m_Type != EventType::INVALID; }
	inline EventType GetType() const { return m_Type; } 
	inline Vertex2 GetPos() const { return m_Position; } 
	inline Vertex2 GetDistance() const { return m_Distance; }
	inline int GetX() const { return m_Position.x; } 
	inline int GetY() const { return m_Position.y; } 
	inline int GetDistanceX() const { return m_Distance.x; }
	inline int GetDistanceY() const { return m_Distance.y; }
};

