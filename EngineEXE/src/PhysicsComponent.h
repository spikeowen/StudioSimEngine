#pragma once
#include "Component.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(Actor* owner, int updateOrder, const float mass, const bool bSimulateGravity, const float gravityMultiplier);
	~PhysicsComponent();

	void SetMass(const float mass) { assert(mass != 0); m_InverseMass = 1 / mass; }
	void SetInverseMass(const float inverseMass) { m_InverseMass = inverseMass; }

	float GetMass()const { return (m_InverseMass == 0) ? std::numeric_limits<float>::max() : 1.0f / m_InverseMass; }
	float GetInverseMass()const { return m_InverseMass; }

	bool HasFiniteMass() const { return m_InverseMass >= 0; }

	/// <summary>
	/// should be less than 1 for the slowing of an object
	/// </summary>
	/// <param name="damp"></param>
	void SetDamping(const float damp) { m_Dampening = damp; }
	float GetDamping() const { return m_Dampening; }

	/// <summary>
	/// This is for the movement like in unity
	/// </summary>
	/// <param name="velocity"></param>
	void SetVelocity(const Vector3& velocity) { m_Velocity = velocity; }
	Vector3 GetVelocity() const { return m_Velocity; }

	/// <summary>
	/// recommend you use for whether the object should be floaty or heavy 
	/// </summary>
	/// <param name="acceleration"></param>
	void SetAcceleration(const Vector3& acceleration) { m_Acceleration = acceleration; }
	Vector3 GetAcceleration() const { return m_Acceleration; }

	void SetGravityValue(const float gravityMultiplier) { m_GravitationalValue = m_baseGravitationalValue * gravityMultiplier; UpdateAccelerationByGravity(); }
	float GetGravity() const { return m_GravitationalValue; }

	virtual void Update(float deltaTime) override;

	void UpdateAccelerationByGravity();

	void ClearAccumulator();

	void SetHitWall(const bool bHitRightWall, const bool bHitLeftWall);

	float GetSpeed() { return  m_Velocity.Length(); }
	Vector3 GetDirection() { return Vector3::Normalize(m_Velocity); }

	void AddForce(const Vector3& force);

	void SetOnGround(const bool bOnGround);

	void SetSimulateGravity(const bool bSimulateGravity);

	void ResetForces();

protected:

	/// <summary>
	///  Holds the linear velocity of the particle in world space
	/// </summary>
	Vector3 m_Velocity;

	/// <summary>
	///  Holds the acceleration of the particle. This value can be used to set acceleration due to gravity (its primary use) or any other constant acceleration.
	/// </summary>
	Vector3 m_Acceleration;

	/// <summary>
	///  Holds the amount of damping applied to linear motion. Damping is required to remove energy added through numerical instability in the integrator.
	/// </summary>
	float m_Dampening;

	/// <summary>
	/// Holds the inverse of the mass of the particle. It is more useful to hold the inverse mass because
	/// integration is simpler and because in real-time simulation it is more useful to have objects with
	/// infinite mass (immovable) than zero mass (completely unstable in numerical simulation).
	/// </summary>
	float m_InverseMass;

	Vector3 m_Gravity;

	Vector3 m_Force;

	float m_GravitationalValue = 9.807f;

	float m_baseGravitationalValue = 9.807f;

	bool m_bOnGround = false;
	bool m_bHitLeftWall = false;
	bool m_bHitRightWall = false;

	bool m_bSimulateGravity = true;

	class CollisionResolver* m_Contacts;
	class CollisionResolver* m_ContactsWall;
};