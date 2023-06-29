#include "pch.h"

#include "Renderer.h"
#include "Quack.h"

std::vector<VertexArray*> Renderer::m_objects;
Shader* Renderer::m_shader;

void Renderer::Init()
{
	m_shader = Quack::GetPrimitiveShader();
}

void Renderer::DrawLine(Vector3 p0, Vector3 p1, Vector3 color)
{
	float vertices[] = { p0.x, p0.y, p0.z, 
						 p1.x, p1.y, p1.z };

	VertexBuffer vb = { vertices, sizeof(float) * 6 };
	VertexBufferLayout vblVer;
	vblVer.Push<float>(3);

	float colors[] = { color.x, color.y, color.z,
					   color.x, color.y, color.z };
	
	VertexBuffer vbc = { colors, sizeof(float) * 6};
	VertexBufferLayout vblCol;
	vblCol.Push<float>(3);

	VertexArray* va = new VertexArray();
	va->AddBuffer(vb, vblVer, VertexType::VERTEX);
	va->AddBuffer(vbc, vblCol, VertexType::COLOR);

	m_objects.push_back(va);
}

void Renderer::DrawDebugLines(OrthographicCamera* camera)
{
	m_shader->Bind();
	m_shader->SetUniform4x4("u_viewProjection", camera->GetViewProjectionMatrix());
	for (VertexArray* line : m_objects)
	{
		line->Bind();
		glDrawArrays(GL_LINES, 0, 2);
		line->Unbind();
	}
	m_shader->Unbind();
}
