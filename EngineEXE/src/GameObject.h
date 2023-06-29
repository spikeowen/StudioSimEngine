#pragma once
#include "BasicIncludes.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "QuackDataTypes.h"
#include "Shader.h"
#include "Transform.h"
#include "QuackPhysics.h"
#include "OrthographicCamera.h"

enum class GameObjectType
{
	OBJECT,
	ACTOR,
	CHARACTER,
	ENEMY,
	ATTACKHITBOX,
	PROJECTILE
};

struct VertexData
{
	std::vector<float> vertices;
	std::vector<float> colors;
	std::vector<float> texCoords;
};

class GameObject
{
public:
	GameObject(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName, Shader* shader);
	GameObject(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName);
	virtual ~GameObject();

	// Rendering
	virtual void Draw(OrthographicCamera* camera);

	// Update
	virtual void Update(const float deltaTime);

	// Position
	virtual void SetPosition(const Vector3 newPosition);
	virtual void AdjustPosition(const Vector3 adjustPosition);
	inline const Vector3 GetPosition() const { if (m_transform) return m_transform->GetPosition(); }
	inline const Vector3 GetPreviousPosition() const { if (m_transform) return m_transform->GetPreviousPosition(); }
	inline TransformData GetTransformData() { return m_transformData; }
	inline Transform* GetTransform() { return m_transform; }

	// Rotation
	virtual Vector3 GetRotation() const { if (m_transform) return m_transform->GetRotation(); }
	virtual void SetRotation(const float angle) { if (m_transform) m_transform->SetRotation(angle); }
	virtual void AdjustRotation(const Vector3 adjustRotation) { if (m_transform) m_transform->AdjustRotation(adjustRotation); }
	virtual void SetRotationAroundPivot(const Vector3 pivotPosition, const float newRotation) { if (m_transform) m_transform->SetRotationAroundPivot(pivotPosition, newRotation); }

	// Scale
	virtual Vector3 GetScale() const { if (m_transform) return m_transform->GetScale(); }
	virtual void SetScale(const Vector3 newScale);
	virtual void AdjustScale(const Vector3 adjustScale);

	// Collision
	inline void SetCollisionData(const CollisionData& newCollisionData) { m_collisionData = newCollisionData; }
	inline const CollisionData& GetCollisionData() const { return m_collisionData; }

	void SetCollisionType(const CollisionType newCollisionType);
	inline const CollisionType GetCollisionType() const { return m_collisionData.collisionType; }

	inline void SetCollisionCenter(const Vector3 newCenterPosition) { m_collisionData.centerPosition = newCenterPosition + m_collisionData.centerPositionOffset; }
	inline void UpdateCollisionCenter() { m_collisionData.centerPosition = GetPosition() + m_collisionData.centerPositionOffset; }
	inline const Vector3 GetCollisionCenter() const { return m_collisionData.centerPosition; }

	void SetCollisionCenterOffset(const Vector3 newCenterPositionOffset);
	inline const Vector3 GetCollisionCenterOffset() const { return m_collisionData.centerPositionOffset; }

	inline void SetCollisionBoxSize(const Vector3 newSize) { m_collisionData.size = newSize; }
	inline const Vector3 GetCollisionBoxSize() const { return m_collisionData.size; }

	inline void SetCollisionSphereRadius(const float newRadius) { m_collisionData.radius = newRadius; }
	inline const float GetCollisionSphereRadius() const { return m_collisionData.radius; }

	const bool GetIsCollidingGameObject(GameObject* gameObject) const;
	virtual void OnCollision(GameObject* collidingObject);
	virtual void OnCollisionOver(GameObject* gameObject);

	// Other
	inline const std::string const GetName() { return m_name; }

	// Object
	inline VertexArray* GetVertexArray() { return m_va;  };
	inline VertexData* GetGameObjectData() { return m_data; }
	inline Texture* GetTexture() const { return m_texture; }
	inline std::string GetTextureName() { return m_textureName; }
	inline void SetTextureName(std::string textureName) { m_textureName = textureName; }

	// Name + type
	inline std::string GetName() const { return m_name; }
	inline void SetName(std::string newName) { m_name = newName; }
	inline GameObjectType GetType() const { return m_type; }

	void UpdateVertexArray();
	void UpdateObjectData(VertexData* newData);

	void SetNewTexture(std::string newPath);
protected:
	// Name + type
	std::string m_name = "";
	GameObjectType m_type;
	
	// Shader
	Shader* m_shader;

	// Texture
	Texture* m_texture = nullptr;
	std::string m_textureName;

	// Object
	VertexArray* m_va = nullptr;
	VertexData* m_data = nullptr;
	Transform* m_transform = nullptr;
	TransformData m_transformData;

	// Collision
	CollisionData m_collisionData = CollisionData();
	std::vector<GameObject*> m_collidingObjects;
};

