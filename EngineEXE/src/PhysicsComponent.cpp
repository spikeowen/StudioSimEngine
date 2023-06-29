#include "pch.h"

#include "PhysicsComponent.h"
#include "CollisionManager.h"

PhysicsComponent::PhysicsComponent(Actor* owner, int updateOrder, const float mass, const bool bSimulateGravity, const float gravityMultiplier) : Component(owner, updateOrder), m_InverseMass(1 / mass)
{
	m_Gravity = Vector3(0, -m_GravitationalValue, 0);
	m_Acceleration = m_InverseMass * (1 / m_InverseMass) * m_Gravity;
	m_Dampening = 0.995f;
	m_Velocity = Vector3(0.0f, 0.0f, 0.0f);
	m_Force = Vector3(0.0f, 0.0f, 0.0f);
	m_owningActor = owner;
	m_updateOrder = updateOrder;
	SetSimulateGravity(bSimulateGravity);
	SetGravityValue(gravityMultiplier);
	m_Contacts = new CollisionResolver();
	m_ContactsWall = new CollisionResolver();
}


PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::Update(float deltaTime)
{
	if (m_InverseMass <= 0.0f || !m_bSimulateGravity) return;

	m_owningActor->AdjustPosition(m_Velocity * deltaTime);

	Vector3 resultingAcc = m_Acceleration;
	resultingAcc += m_Force * m_InverseMass;

	m_Velocity += resultingAcc * deltaTime;

	m_Velocity *= powf(m_Dampening, deltaTime);

	ClearAccumulator();
	if (m_bOnGround)
	{
		m_Contacts->Resolve(deltaTime);
		
	}

	if (m_bHitLeftWall || m_bHitRightWall)
	{
		m_ContactsWall->Resolve(deltaTime);
	}
}

void PhysicsComponent::UpdateAccelerationByGravity()
{
	m_Gravity = Vector3(0, -m_GravitationalValue, 0);
	m_Acceleration = m_InverseMass * (1 / m_InverseMass) * m_Gravity;
}

void PhysicsComponent::ClearAccumulator()
{
	m_Force.Clear();
}

void PhysicsComponent::AddForce(const Vector3& force)
{
	m_Force += force;
}

void PhysicsComponent::SetOnGround(const bool bOnGround)
{
	m_bOnGround = bOnGround;

	if (bOnGround)
	{
		//ResetForces();
		m_Contacts->m_PhysicsObject[0] = this;
		m_Contacts->m_PhysicsObject[1] = nullptr;
		m_Contacts->restitution = 0.05f; 
	}
	else
	{
		//UpdateAccelerationByGravity();
	}
}

void PhysicsComponent::SetSimulateGravity(const bool bSimulateGravity)
{
	m_bSimulateGravity = bSimulateGravity;

	if (!bSimulateGravity)
	{
		ResetForces();
	}
	else
	{
		UpdateAccelerationByGravity();
	}
}

void PhysicsComponent::ResetForces()
{
	//SetAcceleration(Vector3(0, 0, 0));
	SetVelocity(Vector3(0, 0, 0));
}

void PhysicsComponent::SetHitWall(const bool bHitRightWall, const bool bHitLeftWall)
{
	m_bHitRightWall = bHitRightWall;
	m_bHitLeftWall = bHitLeftWall;

	if (m_bHitRightWall)
	{
		//ResetForces();
		m_ContactsWall->m_PhysicsObject[0] = this;
		m_ContactsWall->m_PhysicsObject[1] = nullptr;
		m_ContactsWall->m_contactNormal = Vector3(-5, 0, 0);
		QE_LOG("right side resolve");
		//m_ContactsWall->restitution = 1.0f;

	}
	if (m_bHitLeftWall)
	{
		m_ContactsWall->m_PhysicsObject[0] = this;
		m_ContactsWall->m_PhysicsObject[1] = nullptr;
		m_ContactsWall->m_contactNormal = Vector3(1, 0, 0);
		m_ContactsWall->restitution = 0.0f;
		QE_LOG("left side resolve");

		//UpdateAccelerationByGravity();
	}
}