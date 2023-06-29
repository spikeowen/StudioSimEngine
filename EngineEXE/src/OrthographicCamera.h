#pragma once
#include "BasicIncludes.h"
#include "glm\ext\vector_common.hpp"

class OrthographicCamera
{
public:
	OrthographicCamera(float l, float r, float b, float t);
	OrthographicCamera(float width, float height);

	inline void SetPosition(const glm::vec3& position) { m_position = position; RecalculateViewMatrix(); }
	inline glm::vec3 GetPosition() { return m_position; }

	inline void SetRotation(const float& rotation) { m_rotation = rotation; RecalculateViewMatrix(); }
	inline float GetRotation() { return m_rotation; }

	void SetZoom(float zoom, float aspect);
	inline void ResetZoom() { m_Zoom = m_DefaultZoom; }
	inline float GetZoom() { return m_Zoom; }
	inline float GetGameZoom() { return m_GameZoom; }
	inline float GetZoomSpeed() { return m_ZoomSpeed; }
	inline float GetMoveSpeed() { return m_MoveSpeed; }

	inline void SetCanZoom(bool canZoom) { m_CanZoom = canZoom; }
	inline bool GetCanZoom() { return m_CanZoom; }

	inline void SetDefualtPosition(glm::vec3 pos) { m_defaultPos = pos; }
	inline glm::vec3 GetDefaultPos() { return m_defaultPos; }

	const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
	const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
	const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	const Matrix4 GetViewProjMatrix() const { return m_viewProj; }

	void RecalculateProjection(float l, float r, float b, float t);
private:
	void RecalculateViewMatrix();
private:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_viewProjectionMatrix;
	Matrix4 m_viewProj;

	glm::vec3 m_defaultPos;
	glm::vec3 m_position;
	float m_rotation = 0.0f;

	float m_Zoom;
	float m_DefaultZoom;
	float m_GameZoom;
	float m_ZoomSpeed;
	float m_MoveSpeed;

	bool m_CanZoom : 1;
};
