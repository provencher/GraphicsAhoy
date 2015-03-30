//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "Camera.h"


class StaticCamera : public Camera
{
public:
	StaticCamera(glm::vec3 position, glm::vec3 lookAtPoint, glm::vec3 upVector);
	virtual ~StaticCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;

	virtual glm::vec3 getCamPos(){ return mPosition; }
	virtual glm::vec3 getLookAt(){ return mLookAtPoint; }
	virtual glm::vec3 getCamUpV(){ return mUpVector; }

	virtual void setCamPos(glm::vec3 val){ mPosition = val; }
	virtual void setLookAt(glm::vec3 val){ mLookAtPoint = val; }
	virtual void setCamUpV(glm::vec3 val){ mUpVector = val; }

private:
	glm::vec3 mPosition;
	glm::vec3 mLookAtPoint;
	glm::vec3 mUpVector;
};
