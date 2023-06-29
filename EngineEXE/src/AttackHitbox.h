#pragma once
#include "GameObject.h"

class CombatComponent;

class AttackHitbox : public GameObject
{
public:
	AttackHitbox(std::string name, CombatComponent* owningCombatComponent, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName);
	virtual ~AttackHitbox();

	// Collision
	virtual void OnCollision(GameObject* collidingObject) override;
	virtual void OnCollisionOver(GameObject* gameObject) override;

protected:
	GameObject* m_owningGameObject = nullptr;
	CombatComponent* m_owningCombatComponent = nullptr;
};

