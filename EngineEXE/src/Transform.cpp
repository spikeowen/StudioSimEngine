#include "pch.h"

#include "Transform.h"

Transform::Transform(const Vector3 inPosition, const Vector3 inRotation, const Vector3 inScale ) 
{
	m_position = inPosition;
	m_rotation = inRotation; 
	m_scale = inScale;  
	UpdateTransformMatrix();
}

Transform::Transform() 
{
	m_position = Vector3::Zero;
	m_rotation = Vector3::Zero;
	m_scale = Vector3::Zero;
	UpdateTransformMatrix();
}

Transform::~Transform()
{

}

void Transform::UpdateTransformMatrix()
{
	m_transformationMatrix = Matrix4::CreateScale(m_scale);      
	m_transformationMatrix *= Matrix4::CreateRotationZ(m_fRotation);      
	m_transformationMatrix *= Matrix4::CreateTranslation(Vector3(m_position.x, m_position.y, 0.0f));
}

void Transform::SetPosition(const Vector3 newPosition)
{
	m_previousPosition = m_position;
	m_position = newPosition;
	UpdateTransformMatrix();
}

void Transform::AdjustPosition(const Vector3 adjustPosition)
{
	m_previousPosition = m_position;
	m_position += adjustPosition;
	UpdateTransformMatrix();
}

void Transform::SetRotation(const float newAngle)
{
	m_fRotation = CustomMaths::ToRadians(newAngle);
	UpdateTransformMatrix();
}

void Transform::AdjustRotation(const Vector3 adjustRotation)
{
	m_rotation.x += adjustRotation.x;
	m_rotation.y += adjustRotation.y;
	m_rotation.z += adjustRotation.z;
	UpdateTransformMatrix();
}

void Transform::SetRotationAroundPivot(const Vector3 pivotPosition, const float newRotation)
{
	m_fRotation = newRotation;
	m_transformationMatrix *= Matrix4::CreateRotationZAboutPoint(pivotPosition.x, pivotPosition.y, m_fRotation);

}

void Transform::SetScale(const Vector3 newScale)
{
	m_scale = newScale;
	UpdateTransformMatrix();
}

void Transform::AdjustScale(const Vector3 adjustScale)
{
	m_scale += adjustScale;
	UpdateTransformMatrix();
}
