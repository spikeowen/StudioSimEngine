#include "pch.h"

#include "PontiffBaseState.h"
#include "PontiffSlashState.h"
#include "PontiffChargeState.h"
#include "PontiffUltimateState.h"
#include "PontiffFireState.h"
#include "AIComponent.h"
#include <random>
#include "EngineManager.h"

PontiffBaseState::PontiffBaseState(MiniPontiff* pontiff) : PontiffState(pontiff)
{
	m_timer = 2.0f;
	m_pontiff->StartAnimation("idle");
}

void PontiffBaseState::Update(float deltaTime)
{
	m_currTime += deltaTime;
	m_pontiff->GetAIComponent()->Update(deltaTime);
	m_pontiff->StartAnimation("move");

	float distance = Vector3::Distance(m_pontiff->GetPosition(), EngineManager::GetInputCharacter()->GetPosition());

	if (m_currTime >= m_timer)
	{
		if (distance <= 15.0f)
		{
			int attack = rand() % 3;
			switch (attack)
			{
			case 0:
				m_pontiff->SwitchState(new PontiffSlashState(m_pontiff));
				break;
			case 1:
				m_pontiff->SwitchState(new PontiffChargeState(m_pontiff));
				break;
			case 2:
				m_pontiff->SwitchState(new PontiffFireState(m_pontiff));
				break;
			}
		}
	}
}
