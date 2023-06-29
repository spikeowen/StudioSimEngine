#include "pch.h"

#include "GameObject.h"
#include "Animate.h"
#include "Quack.h"
#include "EngineManager.h"

GameObject::GameObject(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName, Shader* shader) :
	m_name(name),
	m_transform(new Transform(transformData.position, transformData.rotation, transformData.scale)),
	m_collisionData(collisionData),
	m_data(data),
	m_transformData(transformData),
	m_textureName(textureName),
	m_shader(shader)
{
	m_type = GameObjectType::OBJECT;
	m_va = new VertexArray();
	UpdateVertexArray();

	m_texture = Quack::GetTexture(textureName);
}

GameObject::GameObject(std::string name, VertexData* data, const TransformData& transformData, const CollisionData& collisionData, const std::string& textureName) :
	m_name(name),
	m_transform(new Transform(transformData.position, transformData.rotation, transformData.scale)),
	m_collisionData(collisionData),
	m_data(data),
	m_transformData(transformData),
	m_textureName(textureName)
{
	m_type = GameObjectType::OBJECT;
	m_va = new VertexArray();
	UpdateVertexArray();

	m_texture = Quack::GetTexture(textureName);
	m_shader = Quack::GetTextureShader();
}

GameObject::~GameObject()
{
	delete m_va;
	m_va = nullptr;

	delete m_data;
	m_data = nullptr;

	delete m_transform;
	m_transform = nullptr;
}

void GameObject::Draw(OrthographicCamera* camera)
{
	// draw square
	m_shader->Bind();
	m_texture->Bind();
	m_va->Bind();

	m_shader->SetMatrixUniform4("u_world", m_transform->GetTransformationMatrix());
	m_shader->SetUniform4x4("u_viewProjection", camera->GetViewProjectionMatrix());
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	m_shader->Unbind();
	m_texture->UnBind();
	m_va->Unbind();
}

void GameObject::Update(const float deltaTime)
{

}

void GameObject::SetPosition(const Vector3 newPosition)
{
	m_transform->SetPosition(newPosition);
	SetCollisionCenter(newPosition);
}

void GameObject::AdjustPosition(const Vector3 adjustPosition)
{
	m_transform->AdjustPosition(adjustPosition);
	const Vector3 newPosition = m_transform->GetPosition();
	SetCollisionCenter(newPosition);
}

void GameObject::SetScale(const Vector3 newScale)
{
	m_transform->SetScale(newScale);
}

void GameObject::AdjustScale(const Vector3 adjustScale)
{
	m_transform->AdjustScale(adjustScale);
}

void GameObject::SetCollisionType(const CollisionType newCollisionType)
{
	m_collisionData.collisionType = newCollisionType;

	if (newCollisionType != CollisionType::NONE)
	{
		SetCollisionCenter(GetPosition());

		if (newCollisionType == CollisionType::BOX)
		{
			SetCollisionBoxSize(GetScale());
		}
	}
	else
	{
		SetCollisionCenter(Vector3(0.0f));
		SetCollisionBoxSize(Vector3(0.0f));
	}
}

void GameObject::SetCollisionCenterOffset(const Vector3 newCenterPositionOffset)
{
	m_collisionData.centerPositionOffset = newCenterPositionOffset;
	UpdateCollisionCenter();
}

void GameObject::UpdateObjectData(VertexData* newData)
{
	m_data = newData;
	UpdateVertexArray();
}

void GameObject::SetNewTexture(std::string fileName)
{
	if (Quack::GetTexture(fileName))
	{
		m_texture = Quack::GetTexture(fileName);

		m_textureName = fileName;
	}
	else
	{
		QE_LOG("Texture Does Not Exist");
	}
}

const bool GameObject::GetIsCollidingGameObject(GameObject* gameObject) const
{
	bool bFound = false;

	for (GameObject* collidingGameObject : m_collidingObjects)
	{
		if (gameObject == collidingGameObject)
		{
			bFound = true;
			break;
		}
	}

	return bFound;
}

void GameObject::OnCollision(GameObject* collidingObject)
{
	if (collidingObject)
	{
		m_collidingObjects.push_back(collidingObject);
	}
}

void GameObject::OnCollisionOver(GameObject* gameObject)
{
	if (gameObject)
	{
		const int gameObjectIndex = EngineManager::GetGameObjectIndex(gameObject, m_collidingObjects);
		m_collidingObjects.erase(m_collidingObjects.begin() + gameObjectIndex);
	}
}

void GameObject::UpdateVertexArray()
{
	VertexBuffer vertexBuffer = VertexBuffer(m_data->vertices.data(), m_data->vertices.size() * sizeof(float));
	VertexBuffer colorBuffer = VertexBuffer(m_data->colors.data(), m_data->colors.size() * sizeof(float));
	VertexBuffer texCoordsBuffer = VertexBuffer(m_data->texCoords.data(), m_data->texCoords.size() * sizeof(float));

	VertexBufferLayout vertexLayout;
	vertexLayout.Push<float>(3);

	VertexBufferLayout colorLayout;
	colorLayout.Push<float>(3);

	VertexBufferLayout texCoordsLayout;
	texCoordsLayout.Push<float>(2);

	m_va->AddBuffer(vertexBuffer, vertexLayout, VertexType::VERTEX);
	m_va->AddBuffer(colorBuffer, colorLayout, VertexType::COLOR);
	m_va->AddBuffer(texCoordsBuffer, texCoordsLayout, VertexType::TEX_COORDS);
}