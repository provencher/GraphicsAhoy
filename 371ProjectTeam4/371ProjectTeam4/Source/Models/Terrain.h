//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "CubeModel.h"

class Terrain : public CubeModel
{
public:
	//events
	Terrain(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~Terrain();
	virtual void Update(float dt);
	virtual void Draw();

	//meshes
	//void LoadMesh(glm::vec3 size, glm::vec3 color);
	//void DestroyMesh();

	//colors
	//void SetSideColor(char side, glm::vec3 col);
	//void SetColor(vec3 col);

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
	//GLuint loadBMP_custom(ci_string file_path);
	GLuint TextureID;
	GLuint Texture;
};
