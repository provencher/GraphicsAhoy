//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "ParsingHelper.h"

#include <vector>

#include <GLM/glm.hpp>

class Path;
class BSpline;

class Model
{
public:
	Model();
	virtual ~Model();
	//------------------------------------------
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	//------------------------------------------
	void Load(ci_istringstream& iss);
	virtual glm::mat4 GetWorldMatrix() const;
	//------------------------------------------
	void SetPosition(glm::vec3 position);
	void SetScaling(glm::vec3 scaling);
	void SetRotation(glm::vec3 axis, float angleDegrees);
	//--------------------------------------------
	glm::vec3 GetPosition() const		{ return mPosition; }
	glm::vec3 GetScaling() const		{ return mScaling; }
	glm::vec3 GetRotationAxis() const	{ return mRotationAxis; }
	float     GetRotationAngle() const	{ return mRotationAngleInDegrees; }
    ci_string GetName()                 { return mName; }
	float	  GetSpeed();
	void SetSpeed(float spd);
	virtual bool ParseLine(const std::vector<ci_string> &token) = 0;

protected:
	
	//Update Methods
	virtual void updateSpline(float dt);
	virtual void updatePath(float dt);

	ci_string mName; // The model name is mainly for debugging
	glm::vec3 mPosition;
	glm::vec3 mScaling;
	glm::vec3 mRotationAxis;
	float     mRotationAngleInDegrees;


	//Paths =======================================
    float mSpeed;
  
	Path* mPath;
	unsigned int mTargetWaypoint;
	
	BSpline* mSpline;
	float mSplineParameterT;
    
};
