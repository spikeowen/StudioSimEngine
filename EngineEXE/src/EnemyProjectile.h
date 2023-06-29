#pragma once
#include "GameObject.h"
#include "AttackHitbox.h"

class EnemyProjectile : public GameObject
{
public:
	EnemyProjectile(std::string projName, float speed, Vector3 dir, Vector3 pos);
	~EnemyProjectile() {}

	void Update(float deltaTime) override;
	void OnCollision(GameObject* collidingObject) override;
private:
	float m_speed;
	float m_distanceTraveled;
	Vector3 m_direction;
	bool m_isBeingDestroyed;
};

