#include "pch.h"

#include "RangedAIComponent.h"
#include "Quack.h"
#include "EnemyProjectile.h"

RangedAIComponent::RangedAIComponent(Actor* owningActor, const int updateOrder) : AIComponent(owningActor, updateOrder)
{
	m_state = MeleeState::IDLE;
	m_targetRange = 20.0f;
	m_attackRange = 10.0f;
	m_pathUpdateTime = 0.5f;
	m_speed = 3.0f;
	m_attackSpeed = 0.75f;
	m_attackTimer = 0.0f;
	m_animStarted = false;
}

void RangedAIComponent::Update(const float deltaTime)
{
	Vector3 enemyPos = m_owningActor->GetPosition();
	Vector3 playerPos = m_player->GetPosition();

	switch (m_state)
	{
	case MeleeState::IDLE:
		if (Vector3::Distance(enemyPos, playerPos) <= m_targetRange)
		{
			m_path = m_pathFinder->FindPath(enemyPos, playerPos);
			m_state = MeleeState::CHASING;
			break;
		}
		break;
	case MeleeState::CHASING:
		m_timer += deltaTime;

		if (m_timer >= m_pathUpdateTime)
		{
			m_path = m_pathFinder->FindPath(enemyPos, playerPos);
			m_timer = 0.0f;
		}

		if (!m_path.empty())
		{
			if (Vector3::Distance(enemyPos, m_path[0]) < 0.5f)
			{
				m_path.erase(m_path.begin());

				// check if we're arrived at destination
				if (m_path.empty() || Vector3::Distance(enemyPos, playerPos) <= m_attackRange)
				{
					m_state = MeleeState::ATTACKING;
					m_timer = 0.0f;
					break;
				}
			}

			Vector3 dir = Vector3::Direction(enemyPos, m_path[0]);
			m_owningActor->AdjustPosition(dir * deltaTime * m_speed);
		}
		break;
	case MeleeState::ATTACKING:
		m_attackTimer += deltaTime;
		if (m_attackTimer >= m_attackSpeed/2)
		{
			if (!m_animStarted)
			{
				m_owningActor->StartAnimation("attack");
				m_animStarted = true;
			}
		}
		if (m_attackTimer >= m_attackSpeed)
		{
			Quack::GetCurrentScene()->AddGameObject(new EnemyProjectile("IceBlock", 10.0f, Vector3::Direction(enemyPos, playerPos), enemyPos));
			m_attackTimer = 0.0f;
			m_animStarted = false;
		}

		if (Vector3::Distance(enemyPos, playerPos) >= m_attackRange)
		{
			m_state = MeleeState::IDLE;
			break;
		}
		break;
	}
}
