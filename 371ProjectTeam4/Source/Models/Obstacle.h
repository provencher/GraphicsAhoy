
#pragma once

#include "CubeModel.h"
using namespace glm;

class Obstacle : public Model
{
public:
	//Obstacle(vec3 size, char * path);
	Obstacle(vec3 size);
	virtual ~Obstacle();
	virtual void Update(float dt);
	virtual void Draw();


protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
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
