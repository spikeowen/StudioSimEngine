#include "pch.h"

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(FrameBufferSpecificiation fbs) : m_fbs(fbs)
{
	Invalidate();
}

FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &m_rendererID));
	GLCall(glDeleteTextures(1, &m_colorAttachment));
	GLCall(glDeleteTextures(1, &m_depthAttachment));
}

void FrameBuffer::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID));
	glViewport(0, 0, m_fbs.width, m_fbs.height);
}

void FrameBuffer::Unbind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::Resize(float width, float height)
{
	m_fbs.width = width;
	m_fbs.height = height;
	Invalidate();
}

void FrameBuffer::Invalidate()
{
	if (m_rendererID)
	{
		GLCall(glDeleteFramebuffers(1, &m_rendererID));
		GLCall(glDeleteTextures(1, &m_colorAttachment));
		GLCall(glDeleteTextures(1, &m_depthAttachment));
	}

	glCreateFramebuffers(1, &m_rendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_fbs.width, m_fbs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_fbs.width, m_fbs.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

	GLCall(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
