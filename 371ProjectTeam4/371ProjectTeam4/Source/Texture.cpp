#include "Texture.h"
#include "stb_image.h"
#include <iostream>
#include <cassert>

std::map<std::string, TextureData*> Texture::s_resourceMap;

TextureData::TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp, GLenum* attachments)
{
	m_textureID = new GLuint[numTextures];
	m_textureTarget = textureTarget;
	m_numTextures = numTextures;
	m_width = width;
	m_height = height;
	m_frameBuffer = 0;
	m_renderBuffer = 0;

	InitTextures(data, filters, internalFormat, format, clamp);
	InitRenderTargets(attachments);
}

TextureData::~TextureData()
{
	if (*m_textureID) glDeleteTextures(m_numTextures, m_textureID);
	if (m_frameBuffer) glDeleteFramebuffers(1, &m_frameBuffer);
	if (m_renderBuffer) glDeleteRenderbuffers(1, &m_renderBuffer);
	if (m_textureID) delete[] m_textureID;
}

void TextureData::InitTextures(unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp)
{
	glGenTextures(m_numTextures, m_textureID);
	for (int i = 0; i < m_numTextures; i++)
	{
		glBindTexture(m_textureTarget, m_textureID[i]);

		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, filters[i]);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, filters[i]);

		if (clamp)
		{
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		glTexImage2D(m_textureTarget, 0, internalFormat[i], m_width, m_height, 0, format[i], GL_UNSIGNED_BYTE, data[i]);
	}
}

void TextureData::InitRenderTargets(GLenum* attachments)
{
	if (attachments == 0)
		return;

	GLenum drawBuffers[32];      //32 is the max number of bound textures in OpenGL
	assert(m_numTextures <= 32); //Assert to be sure no buffer overrun should occur

	bool hasDepth = false;
	for (int i = 0; i < m_numTextures; i++)
	{
		if (attachments[i] == GL_DEPTH_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
		}
		else
			drawBuffers[i] = attachments[i];

		if (attachments[i] == GL_NONE)
			continue;

		if (m_frameBuffer == 0)
		{
			glGenFramebuffers(1, &m_frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], m_textureTarget, m_textureID[i], 0);
	}

	if (m_frameBuffer == 0)
		return;

	if (!hasDepth)
	{
		glGenRenderbuffers(1, &m_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
	}

	glDrawBuffers(m_numTextures, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer creation failed!" << std::endl;
		assert(false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureData::Bind(int textureNum)
{
	glBindTexture(m_textureTarget, m_textureID[textureNum]);
}

void TextureData::BindAsRenderTarget()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	glViewport(0, 0, m_width, m_height);
}


Texture::Texture(const std::string& fileName, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attachment)
{
	m_fileName = fileName;

	std::map<std::string, TextureData*>::const_iterator it = s_resourceMap.find(fileName);
	if (it != s_resourceMap.end())
	{
		m_textureData = it->second;
		m_textureData->AddReference();
	}
	else
	{
		int x, y, bytesPerPixel;
		unsigned char* data = stbi_load(("./res/textures/" + fileName).c_str(), &x, &y, &bytesPerPixel, 4);

		if (data == NULL)
		{
			std::cerr << "Unable to load texture: " << fileName << std::endl;
		}

		m_textureData = new TextureData(textureTarget, x, y, 1, &data, &filter, &internalFormat, &format, clamp, &attachment);
		stbi_image_free(data);

		s_resourceMap.insert(std::pair<std::string, TextureData*>(fileName, m_textureData));
	}
}

Texture::Texture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter, GLenum internalFormat, GLenum format, bool clamp, GLenum attachment)
{
	m_fileName = "";
	m_textureData = new TextureData(textureTarget, width, height, 1, &data, &filter, &internalFormat, &format, clamp, &attachment);
}

Texture::~Texture()
{
	if (m_textureData && m_textureData->RemoveReference())
	{
		if (m_fileName.length() > 0)
			s_resourceMap.erase(m_fileName);

		delete m_textureData;
	}
}

void Texture::Bind(unsigned int unit) const
{
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	m_textureData->Bind(0);
}

void Texture::BindAsRenderTarget()
{
	m_textureData->BindAsRenderTarget();
}

// OpenGL Tutorial 6
// Taken from www.opengl-tutorial.org
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint Texture::loadDDS(const char * imagepath){

	unsigned char header[124];
	FILE *fp;
	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;
	}
	free(buffer);
	return textureID;
}