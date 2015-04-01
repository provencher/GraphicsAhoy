//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "Model.h"
#include "../Path.h"
#include "../BSpline.h"
#include "../light.h"

class LightModel : public Model {
public:
	LightModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	~LightModel();
	void Update(float dt);
	void Draw();


protected:
	

private:

	//Model vars
	// The vertex format could be different for different types of models
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};
	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
	float rotationSpeed;


	std::vector<Path*> mPath;
    std::vector<BSpline*> mSpline;
	std::vector<Light*> mLight;
};
