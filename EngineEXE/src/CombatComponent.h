#pragma once
#include "component.h"

class Character;
class AttackHitbox;

class CombatComponent : public Component
{
public:
	CombatComponent(Actor* owner, int updateOrder, const WeaponData& startingWeaponData);
	~CombatComponent();

	virtual void Update(const float deltaTime) override;

	virtual void UpdateAttackHitboxPosition(const Vector3& newPosition);

	virtual void LightAttack();
	virtual void HeavyAttack();
	virtual void SpecialAttack();
	virtual void StartAttack(const AttackData& attackData);
	virtual bool const CanAttack() const;
	virtual inline bool GetAttacking() const { return m_battacking; }
	virtual void ForceStopAttack();

	virtual void ResetTimerBools();
	virtual void ActivateDelayTimerFinished();
	virtual void ActiveTimerFinished();
	virtual void DeactivateTimerFinished();
	virtual void AttackFinished();

	virtual inline void SetCurrentWeaponData(const WeaponData& newWeaponData) { m_currentWeaponData = newWeaponData; }
	virtual inline const WeaponData GetCurrentWeaponData() const { return m_currentWeaponData; }

	virtual void CharacterHit(Character* hitCharacter);

protected:
	Character* m_owningCharacter = nullptr;

	WeaponData m_currentWeaponData = WeaponData();
	AttackHitbox* m_attackHitbox = nullptr;
	bool m_battacking = false;
	AttackData m_currentAttackData = AttackData();
	float m_currentAttackTimer = 0.0f;
	FacingDirection m_currentAttackFacingDirection = FacingDirection::RIGHT;
	
	bool m_bactivateDelayTimerFinished = false;
	bool m_bactivateTimerFinished = false;
	bool m_bdeactivateTimerFinished = false;
};

