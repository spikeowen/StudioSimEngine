#include "pch.h"

#include "Actor.h"
#include "Animate.h"
#include "Quack.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"

Actor::Actor(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName, const PhysicsData& physicsData, const AnimationData& animationData)
	: GameObject{ name, data, transformData, collisionData, textureName }, m_physicsData(physicsData), m_animationData(animationData)
{
	m_type = GameObjectType::ACTOR;

	//Animation init
	m_animator = new Animate(this, animationData);
	StartAnimation("idle");

	// Input init
	m_inputComponent = new InputComponent(this, 0);
	AddComponent(m_inputComponent);

	// Physics init
	m_physicsComponent = new PhysicsComponent(this, 1, physicsData.mass, physicsData.bsimulateGravity, physicsData.gravityMultiplier);
	AddComponent(m_physicsComponent);

	// Collision init
	m_groundNames.push_back("ground");
	m_groundNames.push_back("Ground");
	m_groundNames.push_back("floor");
	m_groundNames.push_back("Floor");
	m_groundNames.push_back("platform");
	m_groundNames.push_back("Platform");
}

Actor::~Actor()
{
	delete m_animator;
	m_animator = nullptr;

	for (auto component : m_components)
	{
		delete component;
		component = nullptr;
	}
}

void Actor::Draw(OrthographicCamera* camera)
{
	if (m_animator)
	{
		m_animator->UpdateTextCoord(Quack::GetDeltaTime());
	}

	GameObject::Draw(camera);
}

void Actor::Update(const float deltaTime)
{
	GameObject::Update(deltaTime);

	for (auto component : m_components)
	{
		component->Update(deltaTime);
	}

	if (m_animationDataRowsToAdd > 0)
	{
		for (int i = 0; i < m_animationDataRowsToAdd; i++)
		{
			m_animationData.animationRowData.push_back(AnimationRowData());
		}
		m_animationDataRowsToAdd = 0;
	}

	if (m_animationDataRowsToRemove.size() > 0)
	{
		for (int animIndexToRemove : m_animationDataRowsToRemove)
		{
			m_animationData.animationRowData.erase(m_animationData.animationRowData.begin() + animIndexToRemove);
		}
		m_animationDataRowsToRemove.clear();
	}
}

void Actor::AdjustPosition(const Vector3 adjustPosition)
{
	StartAnimation("move");

	GameObject::AdjustPosition(adjustPosition);
}

void Actor::SetMass(float newMass)
{
	m_physicsData.mass = newMass;

	if (m_physicsComponent)
	{
		m_physicsComponent->SetMass(newMass);
	}
}

void Actor::SetSimulateGravity(bool gravityStatus)
{
	m_physicsData.bsimulateGravity = gravityStatus;

	if (m_physicsComponent)
	{
		m_physicsComponent->SetSimulateGravity(gravityStatus);
	}
}

void Actor::SetGravityMultiplier(const float gravityMultiplier)
{
	m_physicsData.gravityMultiplier = gravityMultiplier;

	if (m_physicsComponent)
	{
		m_physicsComponent->SetGravityValue(gravityMultiplier);
	}
}

inline void Actor::SetAnimationStatus(bool animated)
{
	if (m_animationData.banimated == animated)
	{
		return;
	}

	m_animationData.banimated = animated;

	if (m_animator)
	{
		m_animator->SetAnimationStatus(animated);
	}
}

void Actor::SetAnimationRowData(std::vector<AnimationRowData> newAnimationRowData)
{
	m_animationData.animationRowData = newAnimationRowData;

	if (m_animator)
	{
		m_animator->SetAnimationRowData(newAnimationRowData);
	}
}

void Actor::SetAnimationDataRowName(const int animationIndex, const std::string newName)
{
	if (m_currentAnimationData.name == newName)
	{
		if (m_animator)
		{
			m_animator->SetAnimationName(newName);
		}
	}

	m_animationData.animationRowData[animationIndex].name = newName;
}

const std::string Actor::GetAnimationDataRowName(const int animationIndex)
{
	return m_animationData.animationRowData[animationIndex].name;
}

void Actor::SetAnimationDataRowNumber(const int animationIndex, const int newRowNumber)
{
	m_animationData.animationRowData[animationIndex].rowNumber = newRowNumber;

	if (m_currentAnimationData.name == m_animationData.animationRowData[animationIndex].name)
	{
		if (m_animator)
		{
			m_animator->SetAnimationRowNumber(newRowNumber);
		}
	}
}

const int Actor::GetAnimationDataRowNumber(const int animationIndex)
{
	return m_animationData.animationRowData[animationIndex].rowNumber;
}

void Actor::SetAnimationDataNumberOfColumns(const int animationIndex, const int newNumberOfColumns)
{
	m_animationData.animationRowData[animationIndex].amountOfColumns = newNumberOfColumns;

	if (m_currentAnimationData.name == m_animationData.animationRowData[animationIndex].name)
	{
		if (m_animator)
		{
			m_animator->SetAnimationNumberOfColumns(newNumberOfColumns);
		}
	}
}

const int Actor::GetAnimationDataNumberOfColumns(const int animationIndex)
{
	return m_animationData.animationRowData[animationIndex].amountOfColumns;
}

void Actor::SetAnimationDataPlayRate(const int animationIndex, const float newPlayRate)
{
	m_animationData.animationRowData[animationIndex].playRate = newPlayRate;

	if (m_currentAnimationData.name == m_animationData.animationRowData[animationIndex].name)
	{
		if (m_animator)
		{
			m_animator->SetAnimationPlayRate(newPlayRate);
		}
	}
}

const float Actor::GetAnimationDataPlayRate(const int animationIndex)
{
	return m_animationData.animationRowData[animationIndex].playRate;
}

void Actor::SetAnimationDataLooping(const int animationIndex, const bool newbLooping)
{
	m_animationData.animationRowData[animationIndex].blooping = newbLooping;

	if (m_currentAnimationData.name == m_animationData.animationRowData[animationIndex].name)
	{
		if (m_animator)
		{
			m_animator->SetAnimationLooping(newbLooping);
		}
	}
}

const bool Actor::GetAnimationDataLooping(const int animationIndex)
{
	return m_animationData.animationRowData[animationIndex].blooping;
}

void Actor::SetAnimationDataTotalRows(const int newTotalRows)
{
	m_animationData.totalRows = newTotalRows;

	if (m_animator)
	{
		m_animator->SetAnimationTotalRows(newTotalRows);
	}
}

void Actor::SetAnimationDataTotalColumns(const int newTotalColumns)
{
	m_animationData.totalColumns = newTotalColumns;

	if (m_animator)
	{
		m_animator->SetAnimationTotalColumns(newTotalColumns);
	}
}

void Actor::AddAnimationData()
{
	m_animationDataRowsToAdd++;
}

void Actor::RemoveAnimationData(const int animationIndex)
{
	m_animationDataRowsToRemove.push_back(animationIndex);
}

void Actor::OnAnimationFinished(const AnimationRowData& finishedAnimation)
{
	StartAnimation("idle");
}

void Actor::StartAnimation(const std::string animationName, const bool bForce)
{
	const AnimationRowData& newAnimation = GetAnimationByName(animationName);

	if (GetCurrentAnimation().name != newAnimation.name)
	{
		m_animator->SetAnimation(newAnimation);
	}
}

const AnimationRowData& Actor::GetCurrentAnimation()
{
	return m_animator->GetCurrentAnimation();
}

const AnimationRowData& Actor::GetAnimationByName(std::string name)
{
	for (const AnimationRowData& anim : m_animationData.animationRowData)
	{
		if (anim.name == name)
		{
			return anim;
		}
	}

	return AnimationRowData();
}

void Actor::SetCurrentAnimation(const AnimationRowData& newCurrentAnimation)
{
	m_currentAnimationData = newCurrentAnimation;
	
	if (m_animator)
	{
		m_animator->SetAnimation(newCurrentAnimation);
	}
}

void Actor::OnCollision(GameObject* collidingObject)
{
	if (IsGroundObject(collidingObject))
	{
		if (!HasObjectsCollidingWithName(m_groundNames))
		{
			SetCollidingWithGround(true);
		}
	}

	GameObject::OnCollision(collidingObject);
}

void Actor::OnCollisionOver(GameObject* gameObject)
{
	if (IsGroundObject(gameObject))
	{
		if (!HasObjectsCollidingWithName(m_groundNames))
		{
			SetCollidingWithGround(false);
		}
	}
	
	GameObject::OnCollisionOver(gameObject);
}

void Actor::SetCollidingWithGround(const bool bcollidingWithGround)
{
	m_bcollidingWithGround = bcollidingWithGround;

	if (m_physicsComponent)
	{
		m_physicsComponent->SetOnGround(bcollidingWithGround);
	}
}

const bool Actor::IsGroundObject(GameObject* gameObject) const
{
	bool bGroundObject = false;

	for (std::string objectName : m_groundNames)
	{
		if (objectName == gameObject->GetName())
		{
			bGroundObject = true;
			break;
		}
	}

	return bGroundObject;
}

bool Actor::HasObjectsCollidingWithName(const std::string objectName)
{
	bool bFound = false;

	for (GameObject* collidingObject : m_collidingObjects)
	{
		if (collidingObject->GetName() == objectName)
		{
			bFound = true;
			break;
		}
	}

	return bFound;
}

bool Actor::HasObjectsCollidingWithName(const std::vector<std::string> objectNames)
{
	bool bFound = false;

	for (GameObject* collidingObject : m_collidingObjects)
	{
		for (std::string objectName : objectNames)
		{
			if (collidingObject->GetName() == objectName)
			{
				bFound = true;
				break;
			}
		}
		
	}

	return bFound;
}

void Actor::AddComponent(Component* component)
{
	m_components.push_back(component);
}

void Actor::ClearComponents()
{
	m_components.clear();
}

void Actor::ReorderComponents()
{
	return;
}
