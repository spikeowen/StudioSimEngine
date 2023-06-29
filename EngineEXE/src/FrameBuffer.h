#pragma once
#include "BasicIncludes.h"

struct FrameBufferSpecificiation
{
	uint32_t width, height;
	uint32_t samples;

	bool swapChainTarget = false;
};

class FrameBuffer
{
public:
	FrameBuffer(FrameBufferSpecificiation fbs);
	~FrameBuffer();

	inline uint32_t GetColorAttachment() const { return m_colorAttachment; }
	uint32_t GetID() const { return m_rendererID; }

	void Bind();
	void Unbind();

	void Resize(float width, float height);
private:
	void Invalidate();
private:
	uint32_t m_rendererID;
	uint32_t m_colorAttachment, m_depthAttachment;
	FrameBufferSpecificiation m_fbs;
};

