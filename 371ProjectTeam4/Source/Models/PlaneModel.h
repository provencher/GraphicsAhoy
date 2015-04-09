//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "LightModel.h"
#include "GroupModel.h"

class PlaneModel : public GroupModel
{
public:
	PlaneModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~PlaneModel();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

	
	void MakeBody();
	void MakePropeller();
	void MakeWings();
	void MakeRudder();
	void MakeGuns();
	void MakeThrusters();
	void MakeThrust();

private:
	// The vertex format could be different for different types of models
	glm::vec3 color;
	float mBlack;
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};
	LightModel* mLightModel;
	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
};
