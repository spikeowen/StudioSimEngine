#include "pch.h"

#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const TextureData& textureData) :
	m_rendererID(0), m_filePath(textureData.texturePath), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
{
	GLCall(glGenTextures(1, &m_rendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
	// set the texture wrapping parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	// set texture filtering parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	stbi_set_flip_vertically_on_load(1);
	m_localBuffer = stbi_load(textureData.texturePath.c_str(), &m_width, &m_height, &m_BPP, 0);
	if (m_localBuffer)
	{
		GLenum imageFormat;

		if (m_BPP == 1)
		{
			imageFormat = GL_RED;
		}
		else if (m_BPP == 3)
		{
			imageFormat = GL_RGB;
		}
		else if (m_BPP == 4)
		{
			imageFormat = GL_RGBA;
		}
		else
		{
			QE_LOG("Invalid image type");
		}
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, m_width, m_height, 0, imageFormat, GL_UNSIGNED_BYTE, m_localBuffer));
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(m_localBuffer);
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_rendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

