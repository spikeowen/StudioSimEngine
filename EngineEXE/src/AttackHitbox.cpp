#include "pch.h"

#include "AttackHitBox.h"
#include "CombatComponent.h"
#include "Character.h"

AttackHitbox::AttackHitbox(std::string name,  CombatComponent* owningCombatComponent, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName) : GameObject {name, data, transformData, collisionData, textureName}, m_owningCombatComponent(owningCombatComponent)
{
	m_type = GameObjectType::ATTACKHITBOX;

	// Owning object init
	if (m_owningCombatComponent)
	{
		m_owningGameObject = owningCombatComponent->GetOwningActor();
	}
}

AttackHitbox::~AttackHitbox()
{

}

void AttackHitbox::OnCollision(GameObject* collidingObject)
{
	if (collidingObject != m_owningGameObject)
	{
		Character* hitCharacter = dynamic_cast<Character*>(collidingObject);
		if (hitCharacter)
		{
			m_owningCombatComponent->CharacterHit(hitCharacter);
		}
	}
	
	GameObject::OnCollision(collidingObject);
}

void AttackHitbox::OnCollisionOver(GameObject* gameObject)
{
	GameObject::OnCollisionOver(gameObject);
}
