#include "pch.h"

#include "MiniPontiff.h"
#include "PontiffState.h"
#include "Quack.h"
#include "EnemyProjectile.h"
#include "PontiffBaseState.h"
#include "AIComponent.h"

MiniPontiff::MiniPontiff(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName, const PhysicsData& physicsData, const MovementData& movementData, const EntityData& entityData, const AnimationData& animationData, EnemyType enemyType) : 
	Enemy(name, data, transformData, collisionData, textureName, physicsData, movementData, entityData, animationData, enemyType)
{
	m_enemyType = EnemyType::MINI_PONTIFF;
	m_state = new PontiffBaseState(this);

	WeaponData test = WeaponData("test", "Hitbox.png", AttackData(25.0f, 0.1f, 0.1f, Vector3(2.0f, 2.0f, 2.0f), -0.25f, 0.1f, 0.1f, 2.5f, 0.05f), AttackData(40.0f, 0.1f, 0.1f, Vector3(5.0f, 5.0f, 5.0f), -0.5f, 0.2f, 0.25f, 5.0f, 0.05f)); // todo move
	m_combatComponent = new CombatComponent(this, 0, test);
	AddComponent(m_combatComponent);
}

MiniPontiff::~MiniPontiff()
{

}

void MiniPontiff::Update(const float deltaTime)
{
	for (Component* component : m_components)
	{
		AIComponent* aiComp = dynamic_cast<AIComponent*>(component);

		// Dont update AI component as its manually called in the right state
		if (aiComp == nullptr)
			component->Update(deltaTime);
	}

	m_state->Update(deltaTime);
}

void MiniPontiff::OnCollision(GameObject* collidingObject)
{
	Enemy::OnCollision(collidingObject);
}

void MiniPontiff::OnCollisionOver(GameObject* gameObject)
{
	Enemy::OnCollisionOver(gameObject);	
}

void MiniPontiff::SwitchState(PontiffState* newState)
{
	if (newState)
	{
		delete m_state;
		m_state = newState;
	}
}

void MiniPontiff::Blink(Vector3 pos)
{
	SetPosition(pos);
}

void MiniPontiff::WideSlash(float attackSpeed)
{
	m_combatComponent->LightAttack();
}

void MiniPontiff::FloatAttack(Vector3 dir, float attackSpeed)
{
	m_combatComponent->HeavyAttack();
}

void MiniPontiff::ThrowBlades(float projectileNumber)
{
	cout << "WideSlash" << endl;
}

void MiniPontiff::UltimateAttack(Vector3 posToAttackFrom, float attackSpeed)
{
	cout << "WideSlash" << endl;
}

void MiniPontiff::Wait(float deltaTime, float duration)
{
	static int timer = 0.0f;

	if (timer < duration)
	{
		timer += deltaTime;
	}
	timer = 0.0f;
}
