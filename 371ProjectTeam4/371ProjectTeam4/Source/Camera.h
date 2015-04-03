//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
// Updated by Jordan Rutty
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include <GLM/glm.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update(float dt) = 0;

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewProjectionMatrix() const;
	
	virtual glm::vec3 getCamPos(){ return glm::vec3(0, 0, 0); }


	//getters
	glm::vec3 GetPosition(){return mPosition;}
    glm::vec3 GetLookAt(){return mLookAt;}
    glm::vec3 GetRight(){return mRight;}
    glm::vec3 GetUp(){return mUp;}
	//setters
	void SetPosition(glm::vec3 p){mPosition=p;}
	void SetLookAt(glm::vec3 at){mLookAt = at;}
	void SetRight(glm::vec3 right){mRight = right;}
	void SetUp(glm::vec3 up ){mUp = up;}

protected:
	glm::vec3 mPosition;
    glm::vec3 mLookAt;
    glm::vec3 mRight;
    glm::vec3 mUp;
private:
	// Camera Vectors

};
