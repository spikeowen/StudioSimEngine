#include "pch.h"
#include "PhysicsComponent.h"
#include "CollisionManager.h"
#include "EngineManager.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::Update(const float deltaTime)
{
	int index = 0;

	std::vector<GameObject*> currentSceneGameObjects = Quack::GetCurrentScene()->GetGameObjects();

	// Update collision for each game object
	for (GameObject* gameObject : Quack::GetCurrentScene()->GetGameObjects())
	{
		if (gameObject->GetCollisionType() != CollisionType::NONE)
		{
			std::vector<GameObject*> tempGameObjects = Quack::GetCurrentScene()->GetGameObjects();
			tempGameObjects.erase(tempGameObjects.begin() + index);

			for (GameObject* otherGameObject : tempGameObjects)
			{
				bool bColliding = false;

				switch (gameObject->GetCollisionType())
				{
					case CollisionType::BOX:

						if (otherGameObject->GetCollisionType() == CollisionType::BOX)
						{
							BoundingBox owningBox = BoundingBox(gameObject->GetCollisionCenter(), gameObject->GetCollisionBoxSize());
							BoundingBox otherBox = BoundingBox(otherGameObject->GetCollisionCenter(), otherGameObject->GetCollisionBoxSize());
							bColliding = CollisionHelper::BoxToBox(owningBox, otherBox);
							break;
						}
						
						if (otherGameObject->GetCollisionType() == CollisionType::SPHERE)
						{
							BoundingBox owningBox = BoundingBox(gameObject->GetCollisionCenter(), gameObject->GetCollisionBoxSize());
							BoundingSphere otherSphere = BoundingSphere(otherGameObject->GetCollisionCenter(), otherGameObject->GetCollisionSphereRadius());
							bColliding = CollisionHelper::BoxToSphere(owningBox, otherSphere);
							break;
						}

						break;
					case CollisionType::SPHERE:

						if (otherGameObject->GetCollisionType() == CollisionType::BOX)
						{
							BoundingSphere owningSphere = BoundingSphere(gameObject->GetCollisionCenter(), gameObject->GetCollisionSphereRadius());
							BoundingBox otherBox = BoundingBox(otherGameObject->GetCollisionCenter(), otherGameObject->GetCollisionBoxSize());
							bColliding = CollisionHelper::BoxToSphere(otherBox, owningSphere);
							break;
						}
						
						if (otherGameObject->GetCollisionType() == CollisionType::SPHERE)
						{
							BoundingSphere owningSphere = BoundingSphere(gameObject->GetCollisionCenter(), gameObject->GetCollisionSphereRadius());
							BoundingSphere otherSphere = BoundingSphere(otherGameObject->GetCollisionCenter(), otherGameObject->GetCollisionSphereRadius());
							bColliding = CollisionHelper::SpheretoSphere(owningSphere, otherSphere);
							break;
						}
						break;

					default:
						break;
				}

				//// TODO: box to sphere / sphere to box
				//else if
				//{

				//}

				if (bColliding)
				{
					if (!gameObject->GetIsCollidingGameObject(otherGameObject))
					{
						gameObject->OnCollision(otherGameObject);
					}
				}
				else
				{
					if (gameObject->GetIsCollidingGameObject(otherGameObject))
					{
						gameObject->OnCollisionOver(otherGameObject);
					}
				}
			}
		}

		index++;
	}
}



void CollisionResolver::Resolve(float deltaTime)
{
	ResolveVelocity(deltaTime);
	ResolveInterpenetration(deltaTime);
}

void CollisionResolver::ResolveInterpenetration(float deltaTime)
{
	if (m_Penetration <= 0) return;

	float totalInverseMass = m_PhysicsObject[0]->GetInverseMass();
	if (m_PhysicsObject[1]) totalInverseMass += m_PhysicsObject[1]->GetInverseMass();

	if (totalInverseMass <= 0) return;

	Vector3 movePerIMass = m_contactNormal * (-m_Penetration / totalInverseMass);

	m_PhysicsObject[0]->GetOwningActor()->SetPosition(m_PhysicsObject[0]->GetOwningActor()->GetPosition() + movePerIMass * m_PhysicsObject[0]->GetInverseMass());

	if (m_PhysicsObject[1])
	{
		m_PhysicsObject[1]->GetOwningActor()->SetPosition(m_PhysicsObject[1]->GetOwningActor()->GetPosition() + movePerIMass * m_PhysicsObject[1]->GetInverseMass());
	}
}

float CollisionResolver::CalculateSeparateVelocity() const
{
	Vector3 relativeVelocity = m_PhysicsObject[0]->GetVelocity();
	if (m_PhysicsObject[1])
	{
		relativeVelocity -= m_PhysicsObject[1]->GetVelocity();

	}

	Vector3 contactNormal = m_contactNormal;
	contactNormal.Normalize();
	return Vector3::Dot(relativeVelocity, contactNormal);

}

void CollisionResolver::ResolveVelocity(float deltaTime)
{
	if (m_PhysicsObject[1])
	{
		m_contactNormal = m_PhysicsObject[1]->GetOwningActor()->GetPosition() - m_PhysicsObject[0]->GetOwningActor()->GetPosition();
	}

	float separatingVelocity = CalculateSeparateVelocity();

	if (separatingVelocity > 0)
	{
		return;
	}

	float newSepVelocity = -separatingVelocity * restitution;

	float deltaVelocity = newSepVelocity - separatingVelocity;

	float totalInverseMass = m_PhysicsObject[0]->GetInverseMass();
	if (m_PhysicsObject[1])
	{
		totalInverseMass += m_PhysicsObject[1]->GetInverseMass();
	}
	if (totalInverseMass <= 0)
	{
		return;
	}

	float impulse = deltaVelocity / totalInverseMass;

	Vector3 impulsePerMass = m_contactNormal * impulse;

	m_PhysicsObject[0]->SetVelocity(m_PhysicsObject[0]->GetVelocity() + impulsePerMass * m_PhysicsObject[0]->GetInverseMass());

	if (m_PhysicsObject[1])
	{
		m_PhysicsObject[1]->SetVelocity(m_PhysicsObject[1]->GetVelocity() + impulsePerMass * -m_PhysicsObject[1]->GetInverseMass());
	}
	restitution = 0.0f;
}