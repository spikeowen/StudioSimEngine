#pragma once
#include "PontiffState.h"
#include "CombatComponent.h"

class PontiffSlashState : public PontiffState
{
public:
	PontiffSlashState(MiniPontiff* pontiff);
	~PontiffSlashState() {}

	void Update(float deltaTime) override;
};

