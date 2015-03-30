//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>
#include "Model.h"

class CubeModel : public Model
{
public:
	CubeModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~CubeModel();

	virtual void Update(float dt);
	virtual void Draw();

	void Load(ci_istringstream& iss);
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

	GLuint BitmapLoader(ci_string file_path);
	GLuint TextureID;
	GLuint Texture;


};