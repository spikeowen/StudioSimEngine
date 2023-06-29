#include "pch.h"

#include "OrthographicCamera.h"
#include "glm\ext\matrix_clip_space.hpp"
#include "Quack.h"

OrthographicCamera::OrthographicCamera(float l, float r, float b, float t) :
	m_projectionMatrix(glm::ortho(l, r, b, t, -1.0f, 1.0f)),
	m_viewMatrix(1.0f)
{
	m_Zoom = 5.0f;
	m_DefaultZoom = 5.0f;
	m_ZoomSpeed = 0.25f;
	m_MoveSpeed = 5.0f;
	m_GameZoom = 6.5f;

	m_defaultPos = glm::vec3(0.0f);

	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

OrthographicCamera::OrthographicCamera(float width, float height)
{
	m_viewProj = Matrix4::CreateSimpleViewProj(width, height);
}

void OrthographicCamera::SetZoom(float zoom, float aspect)
{
	m_Zoom = zoom;
	RecalculateProjection(-m_Zoom * aspect, m_Zoom * aspect, -m_Zoom, m_Zoom);
}

void OrthographicCamera::RecalculateProjection(float l, float r, float b, float t)
{
	m_projectionMatrix = glm::ortho(l, r, b, t, -1.0f, 1.0f);

	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

}

void OrthographicCamera::RecalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);
	glm::rotate(transform, m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	m_viewMatrix = glm::inverse(transform);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}