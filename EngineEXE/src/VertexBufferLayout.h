#pragma once
#include "BasicIncludes.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_elements; 
	unsigned int m_stride; // bytes between each element of the array

public:
	VertexBufferLayout() : m_stride(0) {}

	template<typename T>
	void Push(int count)
	{
		std::runtime_error(false);
	}

	template<>
	void Push<float>(int count)
	{
		m_elements.push_back({GL_FLOAT, (unsigned int)count, GL_FALSE});
		m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(int count)
	{
		m_elements.push_back({GL_UNSIGNED_INT, (unsigned int)count, GL_FALSE});
		m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, (unsigned int)count, GL_TRUE });
		m_stride += count * VertexBufferElement::GetSizeOfType(GL_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }
};

