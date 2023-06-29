#pragma once
#include "BasicIncludes.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"

enum class VertexType 
{
	VERTEX,
	COLOR,
	TEX_COORDS,
	LIGHT
};

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const VertexType& type);

	inline void Bind() const { glBindVertexArray(m_rendererID); }
	inline void Unbind() const { glBindVertexArray(0); }
private:
	unsigned int m_rendererID;
};