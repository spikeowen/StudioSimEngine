#pragma once
#include "PontiffState.h"
#include "AttackHitbox.h"

class PontiffUltimateState : public PontiffState
{
public:
	PontiffUltimateState(MiniPontiff* pontiff);
	~PontiffUltimateState() {}

	void Update(float deltaTime) override;
private:
	bool m_fired = false;
};

