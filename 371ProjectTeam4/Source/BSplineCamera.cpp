//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "BSplineCamera.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>
#include <algorithm>


using namespace glm;

BSplineCamera::BSplineCamera(BSpline* spline, float speed)
    : Camera(), mSpline(spline), mSpeed(speed), mSplineParameterT(0.0f)
{
    assert(spline != nullptr);
    mPosition = mSpline->GetPosition(mSplineParameterT);
    
    // @TODO - Use a tangent vector on the spline for the camera lookAt
    mLookAt = mSpline->GetTangent(mSplineParameterT);
	glm::vec3 right = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
   //mUp = glm::cross(right, mLookAt);
   mUp = glm::vec3(0,-1,0);


}


BSplineCamera::~BSplineCamera()
{
}

void BSplineCamera::Update(float dt){
    
	//update SplineParameterT(dist)
	// @TODO - Using the BSpline class, update the position on the spline
    //         Set the mLookAt vector pointing in front (tangent on spline)
    //         And mUp vector being as up as possible
	
	//Increment spline position with Percent of segment completed during time (step distance/length of segment)
	mSplineParameterT = mSpline->Travel(mSplineParameterT, dt*mSpeed);
	//prevent overflow of t param by incrementing
	int cPSize = mSpline->GetControlPointSize();
	if(floor(mSplineParameterT) == cPSize) 
		mSplineParameterT -= cPSize;
		

	//update position
	mPosition = mSpline->GetPosition(mSplineParameterT);


	//Calculate Bases ------------------------------
	glm::vec3 tangent = mSpline->GetTangent(mSplineParameterT);
	glm::vec3 acceleration = mSpline->GetAcceleration(mSplineParameterT);
	mLookAt = glm::normalize(tangent);
	mUp = glm::normalize(glm::cross(glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f)), mLookAt));
	//attemp to tilt camera ------------------------
	//mUp = glm::rotate(mUp, atan(glm::length(acceleration)/mSpeed), mLookAt); //rotate by acceleration along LookAt
	//std::cout<<atan(glm::length(acceleration)/mSpeed) << "\n"; 
	
	
}

glm::mat4 BSplineCamera::GetViewMatrix() const
{
    return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}
