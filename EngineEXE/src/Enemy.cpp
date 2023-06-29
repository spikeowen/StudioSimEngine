#include "pch.h"

#include "Enemy.h"
#include "AIComponent.h"
#include "RangedAIComponent.h"
#include "Quack.h"
#include "Animate.h"

Enemy::Enemy(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName, const PhysicsData& physicsData, const MovementData& movementData, const EntityData& entityData, const AnimationData& animationData, EnemyType enemyType) 
	: Character{ name, data, transformData, collisionData, textureName, physicsData, movementData, entityData, animationData }
{
	m_type = GameObjectType::ENEMY;
	m_enemyType = enemyType;
	switch (m_enemyType)
	{
	case EnemyType::MELEE:
		// AI init
		m_aiComponent = new AIComponent(this, 2);
		AddComponent(m_aiComponent);
		break;
	case EnemyType::RANGED:
		// AI init
		m_aiComponent = new RangedAIComponent(this, 2);
		AddComponent(m_aiComponent);
		break;
	case EnemyType::MINI_PONTIFF:
		m_aiComponent = new AIComponent(this, 2);
		AddComponent(m_aiComponent);
		break;
	}
}

Enemy::~Enemy()
{
}

void Enemy::Update(const float deltaTime)
{
	Character::Update(deltaTime);	
}

void Enemy::OnCollision(GameObject* collidingObject)
{
	Character::OnCollision(collidingObject);
}

void Enemy::OnCollisionOver(GameObject* gameObject)
{
	Character::OnCollisionOver(gameObject);
}
