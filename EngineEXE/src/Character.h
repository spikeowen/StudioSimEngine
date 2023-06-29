#pragma once
#include "Actor.h"
#include "CombatComponent.h"

class Character : public Actor
{
public:
	Character(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName, const PhysicsData& physicsData, const MovementData& movementData, const EntityData& entityData, const AnimationData& animationData, const bool bconsumeInput = false);
	virtual ~Character();

	//Update
	virtual void Update(const float deltaTime) override;

	// Position
	virtual void AdjustPosition(const Vector3 adjustPosition) override;
	
	// Collision
	virtual void OnCollision(GameObject* collidingObject) override;
	virtual void OnCollisionOver(GameObject* gameObject) override;
	virtual const inline bool GetHittingWallLeft() const { return m_bHitLeftWall; }
	virtual const inline bool GetHittingWallRight() const { return m_bHitRightWall; }
	virtual void AdjustPositionCollision(const Vector3 adjustPosition);

	// Jumping
	void Jump();
	virtual inline void SetJumping(const bool bjumping) { m_bjumping = bjumping; }
	virtual inline const bool GetJumping() const { return m_bjumping; }
	virtual inline const float GetJumpHeight() const { return m_movementData.jumpHeight; }
	virtual inline void SetJumpHeight(const float newJumpHeight) { m_movementData.jumpHeight = newJumpHeight; }

	// Movement
	virtual inline float GetMovementSpeed() const { return m_movementData.movementSpeed; }
	virtual inline void SetMovementSpeed(const float newSpeed) { m_movementData.movementSpeed = newSpeed; }
	inline MovementData GetMovementData() { return m_movementData; }
	virtual const inline FacingDirection GetFacingDirection() const { return m_facingDirection; }

	// Animation
	virtual void SetIdleAnimation();
	virtual void OnAnimationFinished(const AnimationRowData& finishedAnimation) override;
	virtual void StartAnimation(const std::string animationName, const bool bForce = false) override;

	// Health
	virtual void TakeDamage(const float amount, const float knockbackAmount, const float knockbackSpeed, const FacingDirection damageDirection);
	virtual void Kill();
	virtual void Die();
	virtual void SetCurrentHealth(const float inHealth);
	virtual void AdjustCurrentHealth(const float adjustAmount);
	virtual const inline float GetCurrentHealth() const { return m_currentHealth; }
	virtual void CheckShouldDie();
	virtual inline void ResetCurrentHealth() { m_currentHealth = m_entityData.maxHealth; }
	virtual void SetMaxHealth(const float newMaxHealth, const bool bUpdateHealth = true);
	virtual const inline float GetMaxHealth() const { return m_entityData.maxHealth; }

	// Entity
	inline EntityData GetEntityData() { return m_entityData; }

	// Input
	virtual inline const bool GetConsumingInput() const { return m_bconsumingInput; }
	virtual inline void SetConsumingInput(const bool bnewConsumingInput) { m_bconsumingInput = bnewConsumingInput; }

	// Combat
	virtual inline const WeaponData GetCurrentWeaponData() const { return m_currentWeaponData; }
	virtual void SetCurrentWeaponData(const WeaponData& newWeaponData);
	void LightAttack();
	void HeavyAttack();
	void SpecialAttack();
	void AttackStarted(const std::string attackType);
	bool GetAttacking() const;

	// Dash
	void AttemptToDash();
	void CheckDash();

protected:
	// Movement
	MovementData m_movementData = MovementData();
	bool m_bjumping = false;
	bool m_CanMove = false;
	bool m_bHitLeftWall = false;
	bool m_bHitRightWall = false;
	FacingDirection m_facingDirection = FacingDirection::RIGHT;
	
	// Entity
	EntityData m_entityData = EntityData();
	float m_currentHealth = 0.0f;
	float m_deathTimer = 2.5f;

	// Input
	bool m_bconsumingInput = false;

	// Combat
	WeaponData m_currentWeaponData = WeaponData();
	bool m_beingKnockbacked = false;
	float m_knockbackSpeed = 0.0f;
	float m_totalKnockbackAmount = 0.0f;
	float m_currentKnockbackAmount = 0.0f;

	// Components
	CombatComponent* m_combatComponent = nullptr;

	//dash
	bool m_IsDashing;
	float m_DashTime;
	float m_DashSpeed;
	float m_DashCooldowm;
	float m_DashTimeLeft;
	float m_TimeSinceLastDash = -100;
};

