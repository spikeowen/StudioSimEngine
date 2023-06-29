#include "pch.h"

#include "EnemyProjectile.h"
#include "Quack.h"
#include "JsonLoader.h"
#include "Transform.h"
#include "EngineManager.h"

EnemyProjectile::EnemyProjectile(std::string projName, float speed, Vector3 dir, Vector3 pos) :
	GameObject(*JsonLoader::LoadGameObject2D(projName)),
	m_speed(speed),
	m_direction(dir),
	m_isBeingDestroyed(false)
{
	VertexData* vertexData = JsonLoader::LoadObjectData2D("Square");
	TransformData transform = TransformData(m_transform->GetPosition(), m_transform->GetRotation(), m_transform->GetScale());
	m_transform->SetPosition(pos);
	m_type = GameObjectType::PROJECTILE;

}

void EnemyProjectile::Update(float deltaTime)
{
	if (!m_isBeingDestroyed)
	{
		Vector3 travel = m_speed * m_direction * deltaTime;

		AdjustPosition(travel);

		m_distanceTraveled += travel.Length();
		if (m_distanceTraveled >= 10.0f)
		{
			Quack::GetCurrentScene()->RemoveGameObject(this);
			m_isBeingDestroyed = true;
		}
	}
}

void EnemyProjectile::OnCollision(GameObject* collidingObject)
{
	if (!m_isBeingDestroyed)
	{
		if (collidingObject->GetType() != GameObjectType::ENEMY && collidingObject->GetType() != GameObjectType::PROJECTILE)
		{
			Quack::GetCurrentScene()->RemoveGameObject(this);
			m_isBeingDestroyed = true;
		}
	}

	if (EngineManager::GetInputCharacter() == collidingObject)
	{
		Character* chara = dynamic_cast<Character*>(collidingObject);
		if (chara)
		{
			chara->TakeDamage(10.0f, 0.1f, 0.1f, FacingDirection::RIGHT);
		}
	}
}
