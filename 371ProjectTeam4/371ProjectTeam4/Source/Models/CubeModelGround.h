

#pragma once

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

#include <GLM/glm.hpp>
#include "CubeModel.h"


class CubeModelGround : public CubeModel
{
public:
	CubeModelGround(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~CubeModelGround();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);



private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int uvbuffer;

	// Load a .BMP filepath using our custom loader
	GLuint loadBMP_custom(ci_string file_path);

	GLuint TextureID;
	GLuint Texture;

};
