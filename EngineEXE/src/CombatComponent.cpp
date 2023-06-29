#include "pch.h"
#include "CombatComponent.h"
#include "Quack.h"
#include "EngineManager.h"
#include "JsonLoader.h"
#include "Character.h"
#include "AttackHitbox.h"
#include "Observer.h"

CombatComponent::CombatComponent(Actor* owner, int updateOrder, const WeaponData& startingWeaponData) : Component {owner, updateOrder}
{
	// Character init
	m_owningCharacter = dynamic_cast<Character*>(owner);

	// Weapon init
	m_currentWeaponData = startingWeaponData;
	EventManager::Instance().PlayMusic.Subscribe([this]() { this->LightAttack(); });
}

CombatComponent::~CombatComponent()
{

}

void CombatComponent::Update(const float deltaTime)
{
	if (m_battacking)
	{
		const float totalAttackTime = m_currentAttackData.totalAttackTime;

		m_currentAttackTimer -= deltaTime;

		if (!m_bactivateDelayTimerFinished && m_currentAttackTimer <= totalAttackTime - m_currentAttackData.hitboxActivateDelay)
		{
			ActivateDelayTimerFinished();
		}
		
		if (!m_bactivateTimerFinished && m_currentAttackTimer <= totalAttackTime - m_currentAttackData.hitboxActiveTime - m_currentAttackData.hitboxActivateDelay)
		{
			ActiveTimerFinished();
		}

		if (!m_bdeactivateTimerFinished && m_currentAttackTimer <= totalAttackTime - m_currentAttackData.hitboxDeactivateTime - m_currentAttackData.hitboxActivateDelay - m_currentAttackData.hitboxActiveTime)
		{
			DeactivateTimerFinished();
		}

		if (m_currentAttackTimer <= 0.0f)
		{
			AttackFinished();
		}
	}
}

void CombatComponent::UpdateAttackHitboxPosition(const Vector3& newPosition)
{
	if (m_battacking && m_attackHitbox)
	{
		const bool RightAttack = m_currentAttackFacingDirection == FacingDirection::RIGHT ? true : false;
		const Vector3 newHitboxPosition = RightAttack ? Vector3((newPosition).x + (m_owningActor->GetCollisionBoxSize().x * 0.5f) + (m_currentAttackData.hitboxScale.x * 0.5f) + m_currentAttackData.sideOffset, newPosition.y, newPosition.z) : Vector3(newPosition.x - (m_owningActor->GetCollisionBoxSize().x * 0.5f) - (m_currentAttackData.hitboxScale.x * 0.5f) - m_currentAttackData.sideOffset, newPosition.y, newPosition.z);
		m_attackHitbox->SetPosition(newHitboxPosition);
	}
}

void CombatComponent::LightAttack()
{
	if (CanAttack())
	{
		Quack::GetAudioEngine()->PlaySound("res/Sounds/LightAttack.wav", Quack::GetAudioEngine()->Attacks(), Vec3{0,0,0}, 0.0f);
		Quack::GetAudioEngine()->PlaySound("res/Sounds/JessicaLight.wav", Quack::GetAudioEngine()->Effects(), Vec3{ 0,0,0 }, 0.0f);
		
		m_owningCharacter->AttackStarted("lightAttack");
		StartAttack(m_currentWeaponData.lightAttackData);
	}
}

void CombatComponent::HeavyAttack()
{
	if (CanAttack())
	{
		Quack::GetAudioEngine()->PlaySound("res/Sounds/HeavyAttack.wav", Quack::GetAudioEngine()->Attacks(), Vec3{0,0,0}, 0.0f);
		Quack::GetAudioEngine()->PlaySound("res/Sounds/JessicaHeavy.wav", Quack::GetAudioEngine()->Effects(), Vec3{ 0,0,0 }, 0.0f);
		m_owningCharacter->AttackStarted("heavyAttack");
		StartAttack(m_currentWeaponData.heavyAttackData);
	}
}

void CombatComponent::SpecialAttack()
{
	if (CanAttack())
	{
		m_owningCharacter->AttackStarted("specialAttack");
		StartAttack(m_currentWeaponData.specialAttackData);
	}
}

void CombatComponent::StartAttack(const AttackData& attackData)
{
	if (attackData.damage > 0.0f && CanAttack())
	{
		m_battacking = true;
		m_currentAttackData = attackData;
		m_currentAttackTimer = attackData.totalAttackTime;
		m_currentAttackFacingDirection = m_owningCharacter->GetFacingDirection();
	}
}

bool const CombatComponent::CanAttack() const
{
	if (!m_battacking)
	{
		return true;
	}

	return false;
}

void CombatComponent::ForceStopAttack()
{
	Quack::GetAudioEngine()->StopChannelGroup(Quack::GetAudioEngine()->Attacks()); 
	ActiveTimerFinished();
	AttackFinished();
}

void CombatComponent::ResetTimerBools()
{
	m_bactivateDelayTimerFinished = false;
	m_bactivateTimerFinished = false;
	m_bdeactivateTimerFinished = false;
}

void CombatComponent::ActivateDelayTimerFinished()
{
	m_bactivateDelayTimerFinished = true;

	const bool RightAttack = m_currentAttackFacingDirection == FacingDirection::RIGHT ? true : false;
	const Vector3 HitboxPosition = RightAttack ? Vector3(m_owningActor->GetCollisionCenter().x + (m_owningActor->GetCollisionBoxSize().x * 0.5f) + (m_currentAttackData.hitboxScale.x * 0.5f) + m_currentAttackData.sideOffset, m_owningActor->GetCollisionCenter().y, m_owningActor->GetCollisionCenter().z) : Vector3(m_owningActor->GetCollisionCenter().x - (m_owningActor->GetCollisionBoxSize().x * 0.5f) - (m_currentAttackData.hitboxScale.x * 0.5f) - m_currentAttackData.sideOffset, m_owningActor->GetCollisionCenter().y, m_owningActor->GetCollisionCenter().z);
	VertexData* weaponHitboxVertexData = QuackEngine::JsonLoader::LoadObjectData2D("Square");

	TransformData weaponHitboxTransformData = TransformData(HitboxPosition, m_owningActor->GetRotation(), m_currentAttackData.hitboxScale);
	CollisionData weaponHitboxCollisionData = CollisionData(weaponHitboxTransformData.position, Vector3(0.0f), weaponHitboxTransformData.scale);

	m_attackHitbox = new AttackHitbox("TemAttackHitbox", this, weaponHitboxVertexData, weaponHitboxTransformData, weaponHitboxCollisionData, m_currentWeaponData.textureName);
	Quack::GetCurrentScene()->AddGameObject(m_attackHitbox);
} 

void CombatComponent::ActiveTimerFinished()
{
	m_bactivateTimerFinished = true;

	if (m_attackHitbox)
	{
		Quack::GetCurrentScene()->RemoveGameObject(m_attackHitbox);
		m_attackHitbox = nullptr;
	}
}

void CombatComponent::DeactivateTimerFinished()
{
	m_bdeactivateTimerFinished = true;
}

void CombatComponent::AttackFinished()
{
	m_battacking = false;
	m_currentAttackTimer = 0.0f;
	ResetTimerBools();
}

void CombatComponent::CharacterHit(Character* hitCharacter)
{
	if (hitCharacter)
	{
		const FacingDirection damageDirection = m_currentAttackFacingDirection == FacingDirection::RIGHT ? FacingDirection::LEFT : FacingDirection::RIGHT;
		hitCharacter->TakeDamage(m_currentAttackData.damage, m_currentAttackData.knockbackAmount, m_currentAttackData.knockbackSpeed, damageDirection);
	}
}
