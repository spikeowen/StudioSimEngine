#include "pch.h"

#include "PontiffChargeState.h"
#include "PontiffBaseState.h"
#include "EngineManager.h"


PontiffChargeState::PontiffChargeState(MiniPontiff* pontiff) : 
	PontiffState(pontiff),
	m_chargeNum(3),
	m_chargeCounter(0),
	m_attacked(false),
	m_teleported(false),
	m_speed(9.0f),
	m_distance(5.0f),
	m_directionChange(false),
	m_chargeDelay(0.25),
	m_delayed(false)
{
	m_player = EngineManager::GetInputCharacter();
	m_timer = 0.7f;
}

void PontiffChargeState::Update(float deltaTime)
{
	// Charge x amount of times
	if (m_chargeCounter < m_chargeNum)
	{
		// Teleport next to the enemy if we haven't this charge
		if (!m_teleported)
		{
			// teleport boss
			Vector3 spawnPos = m_player->GetPosition();
			if (!m_directionChange)
			{
				spawnPos.x -= 2;
				m_directionChange = !m_directionChange;
			}
			else
			{
				spawnPos.x += 2;
				m_directionChange = !m_directionChange;
			}
			m_pontiff->Blink(spawnPos);
			m_originPos = spawnPos;

			// set direction to the player
			m_direction = Vector3::Direction(m_pontiff->GetPosition(), m_player->GetPosition());
			Vector3 currScale;
			currScale = m_pontiff->GetScale();
			if (m_direction.x < 0)
			{
				currScale.x *= -1;
				m_pontiff->SetScale(currScale);
			}
			else
			{
				if (currScale.x < 0)
				{
					currScale.x *= -1;
					m_pontiff->SetScale(currScale);
				}
			}
			m_teleported = true;
		}
		else
		{
			if (!m_attacked)
			{
				// Add a delay before the charge
				if (!m_delayed)
				{
					m_currTime += deltaTime;
					m_pontiff->StartAnimation("idle");
					if (m_currTime >= m_chargeDelay)
					{
						m_delayed = true;
						m_currTime = 0.0f;
					}
				}
				// Charge at the enemy and dissapear
				else
				{
					m_pontiff->StartAnimation("charge");
					m_pontiff->FloatAttack(Vector3(0.0f), -1.0f);
					Vector3 traveledDistance = deltaTime * m_direction * m_speed;
					m_pontiff->AdjustPosition(traveledDistance);
					m_traveled += traveledDistance.Length();
					if (m_traveled >= m_distance)
					{
						m_attacked = true;
						m_delayed = false;
						m_traveled = 0;
						m_chargeCounter++;

						// put pontiff off screen
						m_pontiff->SetPosition({ -999999.0f, 0.0f, 0.0f });
					}
				}
			}
			else
			{
				// delay before starting the next charge
				m_currTime += deltaTime;
				if (m_currTime >= m_timer)
				{
					m_teleported = false;
					m_attacked = false;
					m_currTime = 0.0f;
				}
			}
		}
	}
	else
	{
		// Charges are done so return to base state
		m_currTime += deltaTime;
		if (m_currTime >= m_timer)
		{
			m_pontiff->SetPosition(m_originPos);
			m_pontiff->SwitchState(new PontiffBaseState(m_pontiff));
		}
	}
}
