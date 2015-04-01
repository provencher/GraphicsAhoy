#ifndef TEXTURE_H
#define TEXTURE_H

#include "referenceCounter.h"
#include <GL/glew.h>
#include <string>
#include <map>

class TextureData : public ReferenceCounter
{
public:
	TextureData(GLenum textureTarget, int width, int height, int numTextures, unsigned char** data, GLfloat* filters, GLenum* internalFormat, GLenum* format, bool clamp, GLenum* attachments);

	void Bind(int textureNum);
	void BindAsRenderTarget();

	virtual ~TextureData();
protected:
private:
	TextureData(TextureData& other) {}
	void operator=(TextureData& other) {}

	void InitTextures(unsigned char** data, GLfloat* filter, GLenum* internalFormat, GLenum* format, bool clamp);
	void InitRenderTargets(GLenum* attachments);

	GLuint* m_textureID;
	GLenum m_textureTarget;
	GLuint m_frameBuffer;
	GLuint m_renderBuffer;
	int m_numTextures;
	int m_width;
	int m_height;
};

class Texture
{
public:
	Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clamp = false, GLenum attachment = GL_NONE);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR, GLenum internalFormat = GL_RGBA, GLenum format = GL_RGBA, bool clamp = false, GLenum attachment = GL_NONE);
	virtual ~Texture();

	void Bind(unsigned int unit = 0) const;
	void BindAsRenderTarget();

	// OpenGL Tutorial 6
	// Taken from www.opengl-tutorial.org
	// Load a .BMP file using our custom loader
	// GLuint loadBMP_custom(const char * imagepath);

	// Load a .DDS file using GLFW's own loader
	GLuint loadDDS(const char * imagepath);

protected:
private:
	static std::map<std::string, TextureData*> s_resourceMap;

	Texture(Texture& texture) {}
	void operator=(Texture& texture) {}

	TextureData* m_textureData;
	std::string m_fileName;
};

#endif
