#pragma once
#include "BasicIncludes.h"
#include "QuackPhysics.h"
#include "Actor.h"

using namespace QuackEngine;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void Update(const float deltaTime);
};

class CollisionHelper
{
public:
	/// <summary>
	/// Check if 2 squares collide
	/// </summary>
	/// <param name="box 1"></param>
	/// <param name="box 2"></param>
	/// <returns></returns>
	static bool BoxToBox(BoundingBox b1, BoundingBox b2)
	{
		Vector3 min1 = b1.center - b1.size / 2;
		Vector3 max1 = b1.center + b1.size / 2;
		Vector3 min2 = b2.center - b2.size / 2;
		Vector3 max2 = b2.center + b2.size / 2;

		bool bCollisionX = (min2.x <= min1.x && min1.x <= max2.x) || (min2.x <= max1.x && max1.x <= max2.x);
		bool bCollisionY = (min2.y <= min1.y && min1.y <= max2.y) || (min2.y <= max1.y && max1.y <= max2.y);

		return bCollisionX && bCollisionY;
	}

	/// <summary>
	/// Check if 2 spheres collide
	/// </summary>
	/// <param name="sphere 1"></param>
	/// <param name="sphere 2"></param>
	/// <returns></returns>
	static bool SpheretoSphere(BoundingSphere& sp1, BoundingSphere& sp2)
	{
		float distance = sqrt((sp1.center.x - sp2.center.x) * (sp1.center.x - sp2.center.x) +
							  (sp1.center.y - sp2.center.y) * (sp1.center.y - sp2.center.y) +
							  (sp1.center.z - sp2.center.z) * (sp1.center.z - sp2.center.z));
		return distance <= (sp1.radius + sp2.radius);
	}

	/// <summary>
	/// Check if bounding box collides with sphere collider
	/// </summary>
	/// <param name="b"></param>
	/// <param name="sp"></param>
	/// <returns></returns>
	static bool BoxToSphere(BoundingBox& b, BoundingSphere& sp)
	{
		Vector3 direction = b.center - sp.center;
		direction.Normalize();

		Vector3 checkPoint = sp.center + direction * sp.radius;

		Vector3 min = b.center - b.size / 2;
		Vector3 max = b.center + b.size / 2;

		bool collisionX = min.x <= checkPoint.x && checkPoint.x <= max.x;
		bool collisionY = min.y <= checkPoint.y && checkPoint.y <= max.y;

		return collisionX && collisionY;
	}
private:
	CollisionHelper() {};
	~CollisionHelper() {};
};


class CollisionResolver {
public:


	CollisionResolver() {}

	/// <summary>
	/// Holds the physics that are involved in the contact. The second of these can be NULL, for contacts with the walls, floors etc.
	/// </summary>
	PhysicsComponent* m_PhysicsObject[2];

	float restitution = 0.1f;

	Vector3 m_contactNormal = Vector3(0.0f, 1.0f, 0.0f);

	void Resolve(float deltaTime);

	void ResolveInterpenetration(float deltaTime);

	float m_Penetration;

protected:


	float CalculateSeparateVelocity() const;

private:

	void ResolveVelocity(float deltaTime);

};
