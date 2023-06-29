#pragma once
#include "PontiffState.h"

class Character;

class PontiffChargeState : public PontiffState
{
public:
	PontiffChargeState(MiniPontiff* pontiff);
	~PontiffChargeState() {}

	void Update(float deltaTime) override;
private:
	Character* m_player;
	Vector3 m_direction;
	Vector3 m_originPos;

	float m_distance;
	float m_traveled;
	float m_speed;
	float m_chargeDelay;

	bool m_delayed;
	bool m_attacked;
	bool m_teleported;
	bool m_directionChange;
	int m_chargeNum;
	int m_chargeCounter;
};

