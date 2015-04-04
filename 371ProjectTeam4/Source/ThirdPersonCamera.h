//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "Camera.h"
#include "BSpline.h"

class ThirdPersonCamera : public Camera
{
public:
    ThirdPersonCamera(Model* targetModel);
    virtual ~ThirdPersonCamera();

    virtual void Update(float dt);
    virtual glm::mat4 GetViewMatrix() const;
	virtual void SetCameraRadius(float r);
	virtual float GetCameraRadius();

	virtual glm::vec3 getCamPos(){ return mPosition; }	

protected:
	virtual void UpdateTargeModel(float dt);
	virtual void updateCameraLookAt();

private:
	void collideChildren(Model* collider, std::map <ci_string, Model*>* children);
    Model* mTargetModel;
    // Cartesian Coordinates
    float mHorizontalAngle;
    float mVerticalAngle;
    float mRadius;
    // Camera Vectors
    glm::vec3 mPosition;
    glm::vec3 mLookAt;
    glm::vec3 mRight;
    glm::vec3 mUp;
	//Player
	//Flying
	glm::vec3 mVelocity;
    // @TODO - Maybe you should add more camera attributes (Speed, etc)
};
