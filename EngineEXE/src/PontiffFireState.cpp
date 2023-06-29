#include "pch.h"

#include "PontiffFireState.h"
#include "PontiffBaseState.h"
#include "EnemyProjectile.h"
#include "Quack.h"

PontiffFireState::PontiffFireState(MiniPontiff* pontiff) : PontiffState(pontiff)
{
	m_timer = 1.0f;
}

void PontiffFireState::Update(float deltaTime)
{
	if (!m_fired)
	{
		Quack::GetCurrentScene()->AddGameObject(new EnemyProjectile("IceBlock", 10.0f, { 1.0f, 0.0f, 0.0f }, m_pontiff->GetPosition()));
		Quack::GetCurrentScene()->AddGameObject(new EnemyProjectile("IceBlock", 10.0f, { 0.0f, 1.0f, 0.0f }, m_pontiff->GetPosition()));
		Quack::GetCurrentScene()->AddGameObject(new EnemyProjectile("IceBlock", 10.0f, { -1.0f, 0.0f, 0.0f }, m_pontiff->GetPosition()));
		Quack::GetCurrentScene()->AddGameObject(new EnemyProjectile("IceBlock", 10.0f, { 0.0f, -1.0f, 0.0f }, m_pontiff->GetPosition()));
		Quack::GetCurrentScene()->AddGameObject(new EnemyProjectile("IceBlock", 10.0f, { 0.5f, 0.5f, 0.0f }, m_pontiff->GetPosition()));
		Quack::GetCurrentScene()->AddGameObject(new EnemyProjectile("IceBlock", 10.0f, { 0.5f, -0.5f, 0.0f }, m_pontiff->GetPosition()));
		Quack::GetCurrentScene()->AddGameObject(new EnemyProjectile("IceBlock", 10.0f, { -0.5f, 0.5f, 0.0f }, m_pontiff->GetPosition()));
		Quack::GetCurrentScene()->AddGameObject(new EnemyProjectile("IceBlock", 10.0f, { -0.5f, -0.5f, 0.0f }, m_pontiff->GetPosition()));
		m_fired = true;
	}
	else
	{
		m_currTime += deltaTime;
		if (m_currTime >= m_timer)
		{
			m_pontiff->SwitchState(new PontiffBaseState(m_pontiff));
		}
	}
}
