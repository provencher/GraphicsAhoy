//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
// Hierarchy, Color and Transforms Added by Jordan Rutty
//

#pragma once

#include "../ParsingHelper.h"
#include "../Tools/ray.h"
#include <vector>
#include <unordered_map>
#include <GLM/glm.hpp>
#include <map>
class Path;
class BSpline;
//////////////////////////////////////////////////////////////////////////
//								Model
//=======================================================================
//mPath;World::FindPath("RollerCoaster");
class Model{
public:		//------------------------------------------
	Model();
	glm::vec4 materialConst;
	//Events------------------------------------
	void		 Load(ci_istringstream& iss);
	virtual	bool ParseLine(const std::vector<ci_string> &token) = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual ~Model();
	//Identity----------------------------------
	virtual		glm::mat4	GetWorldMatrix();
	ci_string	GetName(){ return mName; }
	void		SetName(ci_string name){ mName=name; }
	ci_string	mType; //model, sphere, cube, plane,
	ci_string	mShaderName; //name used to draw object
	//Transforms ------------------------------------------
	glm::mat4	transform;	//apply transform to model first before trs
	//T
	void		SetPosition(glm::vec3 position);
	glm::vec3	GetPosition()		const	{ return mPosition; }
	void		SetRotation(glm::vec3 axis, float angleDegrees);
	//R
	glm::vec3	GetRotationAxis()	const	{ return mRotationAxis; }
	float		GetRotationAngle()	const	{ return mRotationAngleInDegrees; }
	float		mRotationAngleX;
	float		mRotationAngleY;
	float		mRotationAngleZ;
	//S
	void		SetScaling(glm::vec3 scaling);
	glm::vec3	GetScaling()		const	{ return mScaling; }
	//----------------------------------------------------------
	void		SetSideColor(glm::vec3 col);			//#Idea best implemented though OpenGl color 
    //----------------------------------------------------------
	float		GetSpeed();
	void		SetSpeed(float spd);
	void		SetSpline(BSpline* sp);
	BSpline*	GetSpline();
	void		SetSplineParameterT(float t);
	// Hierarchy ------------------------------------------------
	std::map <ci_string, Model*> child;
	Model*	Parent();
	bool	HasParent();
	void	SetParent(Model* m);
	bool	HasChild(ci_string key);
	void	AddChild(Model* m);					//Auto Asign name to_string(++(#spawned))
	void	AddChild(ci_string key, Model* m);	//Give name & Key accosiation
	void	DeleteChild(ci_string key);			//deleted Completley
	Model*	RemoveChild(ci_string key);			//Removed from Hierarchy
	int		GetChildCount() const				{ return child.size(); }
	void	UpdateChildren(float dt);			//Rectusivly update the nodes children | note: can be modified to only update when activly called, by disabling in the Update()
	void	DrawChildren();						//Must be called in extending class to draw
	void	DeleteAllChildren();
	
	

	
	// Collision ----------------------------------------------
	bool      CollideWith(Model* other);
	void      CreateDefaultCollisionCube();
	void      ReScaleCollisionCube(glm::vec3 newScale);
	void      Intersect(const Ray& ray, std::vector<std::pair<Model*, glm::vec3>>& intersectionPoints);
protected: //------------------------------------------
	// Hierarchy --------------------------------
	Model*	mParent;
	int		mNthChild;// used for naming unnamed children, counter incereses for every spawn, never decreases

	//Update Methods
	virtual void updateSpline(float dt);
	virtual void updatePath(float dt);
	ci_string mName; // The model name is mainly for debugging
	glm::vec3 mPosition;
	glm::vec3 mScaling;
	glm::vec3 mRotationAxis;
	float     mRotationAngleInDegrees;
	glm::vec3* mCollisionCube; // can be null;
	
	//Paths =======================================
    float mSpeed;
  
	Path* mPath;
	unsigned int mTargetWaypoint;
	
	BSpline* mSpline;
	float mSplineParameterT;
};
