#pragma once
#include "Enemy.h"
#include "RangedAIComponent.h"

class PontiffState;

class MiniPontiff : public Enemy
{
public:
	MiniPontiff(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName, const PhysicsData& physicsData, const MovementData& movementData, const EntityData& entityData, const AnimationData& animationData, EnemyType enemyType);
	~MiniPontiff();

	void Update(const float deltaTime) override;
	void OnCollision(GameObject* collidingObject) override;
	void OnCollisionOver(GameObject* gameObject) override;

	void SwitchState(PontiffState* newState);

	// Teleports to a position in the world
	void Blink(Vector3 pos);
	// A slash attack in both directions
	void WideSlash(float attackSpeed);

	// An attack where mini pontiff picks a direction and chases while cutting with his blade
	void FloatAttack(Vector3 dir, float attackSpeed);

	// An attack where pontiff moves to the top of the room and starts throwing blades at the player
	void ThrowBlades(float projectileNumber);

	// An ultimate Attack where pontiff quickly descends down and launches his blades
	void UltimateAttack(Vector3 posToAttackFrom, float attackSpeed);

private:
	// Stand menacingly
	void Wait(float deltaTime, float duration);
private:
	PontiffState* m_state;

	CombatComponent* m_combatComponent = nullptr;
};

