#pragma once
#include "BasicIncludes.h"
#include "CustomMaths.h"

class Transform
{
public:
	Transform(const Vector3 inPosition, const Vector3 inRotation, const Vector3 inScale);
	Transform();
	~Transform();

	// Position
	void SetPosition(const Vector3 newPosition);
	void AdjustPosition(const Vector3 adjustPosition);
	inline const Vector3 GetPosition() const { return m_position; }
	inline const Vector3 GetPreviousPosition() const { return m_previousPosition; }

	// Rotation
	void SetRotation(const float newAngle);
	void AdjustRotation(const Vector3 adjustRotation);
	void SetRotationAroundPivot(const Vector3 pivotPosition, const float newRotation);
	inline const Vector3 GetRotation() const { return m_rotation; }
	inline const float GetFloatRotation() const { return m_fRotation; }

	// Scale
	void SetScale(const Vector3 newScale);
	void AdjustScale(const Vector3 adjustScale);
	inline const Vector3 GetScale() const { return m_scale; }

	//Direction
	inline Vector3 GetForward() const{return Vector3::Normalize(Vector3(cos(m_fRotation), sin(m_fRotation), 0.0f));}
	inline Vector3 GetUp() const{return Vector3::Normalize( Vector3(cos(m_fRotation+90), sin(m_fRotation+90), 0.0f));}

	// Transformation matrix
	inline const Matrix4 GetTransformationMatrix() const { return m_transformationMatrix; }
	// Update the matrix transform, used in draw
	void UpdateTransformMatrix();
	
	

private:
	Matrix4 m_transformationMatrix = Matrix4::Identity;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_previousPosition = Vector3::Zero;
	Vector3 m_rotation = Vector3::Zero;
	float m_fRotation = 0.0f;
	Vector3 m_scale = Vector3::One;

};

