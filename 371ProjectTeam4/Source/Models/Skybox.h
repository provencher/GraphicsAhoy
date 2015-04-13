
#pragma once

#include "Model.h"

class Skybox : public Model
{
public:
	Skybox();
	virtual ~Skybox();
	virtual void Update(float dt);
	virtual void Draw();


protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	GLuint Texture;

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	unsigned int uvbuffer;
};
