#include "pch.h"

#include "PerspectiveCamera.h"


PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 at, glm::vec3 up, float windowWidth, float windowHeight, float nearDepth, float farDepth)
{
	SetEye(position);
	SetAt(at);
	SetUp(up);

	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_nearDepth = nearDepth;
	_farDepth = farDepth;

}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::SetEye(glm::vec3 EyeToSet)
{
	m_eye = EyeToSet;
}

void PerspectiveCamera::SetAt(glm::vec3 AtToSet)
{
	m_at = AtToSet;
}

void PerspectiveCamera::SetUp(glm::vec3 UpToSet)
{
	m_up = UpToSet;
}

glm::vec3 PerspectiveCamera::GetEye()
{
	return this->m_eye;
}

glm::mat4 PerspectiveCamera::GetViewMatrix()
{
	m_viewMatrix = glm::lookAtLH(m_eye, m_at, m_up);
	return m_viewMatrix;
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix()
{
	m_projectionMatrix = glm::perspectiveFovLH(0.01745329f * 130.0f, _windowWidth, _windowHeight, _nearDepth, _farDepth);
	return m_projectionMatrix;
}
void PerspectiveCamera::Recalculate()
{
	m_viewMatrix = glm::lookAtLH(m_eye, m_at, m_up);
	m_projectionMatrix = glm::perspectiveFovLH(0.01745329f * 130.0f, _windowWidth, _windowHeight, _nearDepth, _farDepth);
	m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}