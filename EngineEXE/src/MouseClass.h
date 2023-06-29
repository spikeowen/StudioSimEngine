#pragma once

#include "MouseEvent.h"

class MouseClass
{
private:
	static std::queue<MouseEvent> s_EventBuffer;

	static bool s_IsLeftClick;
	static bool s_IsRightClick;
	static bool s_IsScrollClick;

	static Vertex2 s_Position, s_Distance;
	static float s_DiffX, s_DiffY;
public:
	static void Init();
	static void OnLeftClick(int x, int y);
	static void OnLeftRelease(int x, int y);
	static void OnLeftHeld(int x, int y);
	static void OnRightClick(int x, int y);
	static void OnRightRelease(int x, int y);
	static void OnRightHeld(int x, int y);
	static void OnScrollClick(int x, int y);
	static void OnScrollRelease(int x, int y);
	static void OnScrollHeld(int x, int y);
	static void OnScrollUp(int x, int y);
	static void OnScrollDown(int x, int y);
	static void OnMouseMove(int x, int y);
	static void OnMouseMoveRaw(int x, int y);
	
	static void ResetMousePos(int x, int y);

	static inline bool IsLeftClick() { return s_IsLeftClick; }
	static inline bool IsRightClick() { return s_IsRightClick; }
	static inline bool IsScrollClick() { return s_IsScrollClick; }
	
	static inline int GetPosX() { return s_Position.x; }
	static inline int GetPosY() { return s_Position.y; }
	static inline Vertex2 GetPos() { return s_Position; }
	static inline Vertex2 GetDis() { return s_Distance; }
	static inline float GetDX() { return s_DiffX; }
	static inline float GetDY() { return s_DiffY; }
	
	static inline bool IsEventBufferEmpty() { return s_EventBuffer.empty(); }
	static MouseEvent ReadEvent();
};

