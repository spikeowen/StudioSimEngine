#pragma once
#include "BasicIncludes.h"
#include "VertexArray.h"
#include "Shader.h"
#include "OrthographicCamera.h"

class Renderer
{
public:
	static void Init();
	static void DrawLine(Vector3 p0, Vector3 p1, Vector3 color);
	static void DrawDebugLines(OrthographicCamera* camera);
private:
	static std::vector<VertexArray*> m_objects;
	static Shader* m_shader;
};

