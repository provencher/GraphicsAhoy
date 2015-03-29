//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "GroupModel.h"
#include "../Camera.h"

class BillBoard : public GroupModel
{
public:
	BillBoard(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~BillBoard();
	virtual	glm::mat4	GetWorldMatrix();
	void SetLookAtCamera(Camera* c); //returns old lookat
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


	Camera* mLookAtCamera;
	//initialize rotation ---------------------------
	float mHorizontalAngle; 
	float mVerticalAngle;
	glm::vec3 mLookAt;
	glm::vec3 mRight;
    glm::vec3 mUp;
};
