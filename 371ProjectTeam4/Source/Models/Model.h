//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "../ParsingHelper.h"

#include <vector>
#include <unordered_map>
#include <GLM/glm.hpp>

class Path;
class BSpline;
//////////////////////////////////////////////////////////////////////////
//								Model
//=======================================================================
//mPath;World::FindPath("RollerCoaster");
class Model{
public:		//------------------------------------------
	Model();
	//Events------------------------------------
	void				Load(ci_istringstream& iss);
	virtual	bool		ParseLine(const std::vector<ci_string> &token) = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual ~Model();
	//Identity----------------------------------
	virtual	glm::mat4	GetWorldMatrix();
	ci_string GetName(){ return mName; }
	//T ------------------------------------------
	void		SetPosition(glm::vec3 position);
	glm::vec3	GetPosition()		const	{ return mPosition; }
	//R------------------------------------------
	void		SetRotation(glm::vec3 axis, float angleDegrees);
	glm::vec3	GetRotationAxis()	const	{ return mRotationAxis; }
	float		GetRotationAngle()	const	{ return mRotationAngleInDegrees; }
	//S------------------------------------------
	void		SetScaling(glm::vec3 scaling);
	glm::vec3	GetScaling()		const	{ return mScaling; }
	//--------------------------------------------
	void		SetSideColor(glm::vec3 col);			//#Idea		possibly best set through 
    //--------------------------------------------
	float		GetSpeed();
	void		SetSpeed(float spd);
	// Children --------------------------------
	void	SetParent(Model* m);
	void	AddChild(Model* m);	
	Model*	RemoveChild(Model* m);
	int		GetChildCount() const { return mChildren.size(); }
	void UpdateChildren(float dt);
	void DrawChildren();
	bool HasParent();
	Model* Parent();

protected: //------------------------------------------
	//Children 
	Model* mParent;
	//Update Methods
	virtual void updateSpline(float dt);
	virtual void updatePath(float dt);
	ci_string mName; // The model name is mainly for debugging
	glm::vec3 mPosition;
	glm::vec3 mScaling;
	glm::vec3 mRotationAxis;
	float     mRotationAngleInDegrees;
	
	std::vector<Model*> mChildren;

	//Paths =======================================
    float mSpeed;
  
	Path* mPath;
	unsigned int mTargetWaypoint;
	
	BSpline* mSpline;
	float mSplineParameterT;
    
};
