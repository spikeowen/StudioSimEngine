#pragma once
#include "PontiffState.h"

class PontiffFireState : public PontiffState
{
public:
	PontiffFireState(MiniPontiff* pontiff);
	~PontiffFireState() {}

	void Update(float deltaTime) override;
private:
	bool m_fired = false;
};

