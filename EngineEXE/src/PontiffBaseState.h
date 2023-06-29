#pragma once
#include "PontiffState.h"

class PontiffBaseState : public PontiffState
{
public:
	PontiffBaseState(MiniPontiff* pontiff);
	~PontiffBaseState() {}

	void Update(float deltaTime) override;
};

