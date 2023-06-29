#pragma once
#include "MiniPontiff.h"

class PontiffState
{
public:
	PontiffState(MiniPontiff* pontiff) : m_pontiff(pontiff), m_currTime(0.0f) { }
	virtual ~PontiffState() {}
	virtual void Update(float deltaTime) { }
protected:
	MiniPontiff* m_pontiff;

	float m_currTime;
	float m_timer;
};

