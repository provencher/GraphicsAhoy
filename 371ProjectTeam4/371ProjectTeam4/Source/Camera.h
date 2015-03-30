//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
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
	virtual glm::vec3 getLookAt(){ return glm::vec3(0, 0, 0); }
	virtual glm::vec3 getCamUpV(){ return glm::vec3(0, 0, 0); }

	virtual void setCamPos(glm::vec3 val){}
	virtual void setLookAt(glm::vec3 val){}
	virtual void setCamUpV(glm::vec3 val){}

private:

};
