#pragma once
#include "Character.h"

class AIComponent;

enum class EnemyType
{
	MELEE,
	RANGED,
	MINI_PONTIFF
};

class Enemy : public Character
{
public:
	Enemy(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName, const PhysicsData& physicsData, const MovementData& movementData, const EntityData& entityData, const AnimationData& animationData, EnemyType enemyType);
	virtual ~Enemy();

	// Update
	virtual void Update(const float deltaTime) override;

	// Collision
	virtual void OnCollision(GameObject* collidingObject) override;
	virtual void OnCollisionOver(GameObject* gameObject) override;
	virtual AIComponent* GetAIComponent() { return m_aiComponent; }

	inline EnemyType GetEnemyType() const { return m_enemyType; }
protected:
	// AI
	AIComponent* m_aiComponent = nullptr;
	EnemyType m_enemyType;
};

