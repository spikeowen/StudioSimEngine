#pragma once
#include "BasicIncludes.h"

class PerspectiveCamera
{
public:
	PerspectiveCamera(glm::vec3 position, glm::vec3 at, glm::vec3 up, float windowWidth, float windowHeight, float nearDepth, float farDepth);
	~PerspectiveCamera();

	inline void SetPosition(const glm::vec3& position) { m_position = position; Recalculate(); }
	inline glm::vec3 GetPosition() { return m_position; }

	inline void SetRotation(const float& rotation) { m_rotation = rotation; }
	inline float GetRotation() { return m_rotation; }

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewProjectionMatrix() { return m_viewProjectionMatrix; }
	void Recalculate();

	void SetEye(glm::vec3 EyeToSet);
	void SetAt(glm::vec3 AtToSet);
	void SetUp(glm::vec3 UpToSet);

	glm::vec3 GetEye();


private:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_viewProjectionMatrix;

	glm::vec3 m_eye;
	glm::vec3 m_at;
	glm::vec3 m_up;

	glm::vec3 m_position;
	float m_rotation = 0.0f;

	float _windowWidth;
	float _windowHeight;
	float _nearDepth;
	float _farDepth;
};

