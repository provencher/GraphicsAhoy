
#pragma once

#include "CubeModel.h"

class Craters : public Model
{
public:
	//events
	Craters(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~Craters();
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
	GLuint Texture;
	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int uvbuffer;
};
