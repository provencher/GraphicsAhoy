//
// COMP 371 Project Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
// Jordan
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Model.h"
#include "../Path.h"
#include "../BSpline.h"
#include "../World.h"
//#include "StaticCamera.h"
#include "../Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

using namespace std;
using namespace glm;

//Construct Destruct
Model::Model() : mName("UNNAMED"), mPosition(0.0f, 0.0f, 0.0f), mScaling(1.0f, 1.0f, 1.0f), mRotationAxis(0.0f, 1.0f, 0.0f), mRotationAngleInDegrees(0.0f), mPath(nullptr), mSpeed(0.0f), mTargetWaypoint(1), mSpline(nullptr), mSplineParameterT(0.0f)
{
	mParent = nullptr;

}
Model::~Model()
{
}

//Init----------------------------------------
void Model::Load(ci_istringstream& iss){
	ci_string line;

	// Parse model line by line
	while(std::getline(iss, line)){
		// Splitting line into tokens
		ci_istringstream strstr(line);
		istream_iterator<ci_string, char, ci_char_traits> it(strstr);
		istream_iterator<ci_string, char, ci_char_traits> end;
		vector<ci_string> token(it, end);

		if (ParseLine(token) == false){
			fprintf(stderr, "Error loading scene file... token:  %s!", token[0]);
			getchar();
			exit(-1);
		}
	}
}//Load from file
bool Model::ParseLine(const std::vector<ci_string> &token){
	if (token.empty() == false){
		if (token[0].empty() == false && token[0][0] == '#'){
			return true;
		} else if (token[0] == "name"){
			assert(token.size() > 2);
			assert(token[1] == "=");

			mName = token[2];	
		} else if (token[0] == "position"){
			assert(token.size() > 4);
			assert(token[1] == "=");
			SetPosition(vec3(
				static_cast<float>(atof(token[2].c_str())),
				static_cast<float>(atof(token[3].c_str())),
				static_cast<float>(atof(token[4].c_str()))
			));
		} else if (token[0] == "rotation"){
			assert(token.size() > 4);
			assert(token[1] == "=");

			SetRotation(vec3(
				static_cast<float>(atof(token[2].c_str())),
				static_cast<float>(atof(token[3].c_str())),
				static_cast<float>(atof(token[4].c_str()))),
				static_cast<float>(atof(token[5].c_str()))  //angle
			);

			glm::normalize(mRotationAxis);
		} else if (token[0] == "scaling"){
			assert(token.size() > 4);
			assert(token[1] == "=");
			SetScaling(vec3(
				static_cast<float>(atof(token[2].c_str())),
				static_cast<float>(atof(token[3].c_str())),
				static_cast<float>(atof(token[4].c_str()))
			));
		} else if (token[0] == "pathspeed"){
			assert(token.size() > 2);
			assert(token[1] == "=");

            float speed = static_cast<float>(atof(token[2].c_str()));
            SetSpeed(speed);
		} else if (token[0] == "boundpath"){
			assert(token.size() > 2);
			assert(token[1] == "=");

			ci_string pathName = token[2];
            World* w = World::GetInstance();
            mPath = w->FindPath(pathName);

			if (mPath == nullptr){
				mSpline = w->FindSpline(pathName);
			}
			
			if (mPath != nullptr){
				mPosition = mPath->GetWaypoint(0);
			} else if (mSpline){
				mPosition = mSpline->GetPosition(mSplineParameterT);
			}
		} else {
			return false;
		}
	}

	return true;
}//parse line of text 
//Update--------------------------------------
void Model::Update(float dt){

	if(mPath != nullptr)		
		updatePath(dt);		//Move along Path
	else if (mSpline)		
		updateSpline(dt);	//Move along Spline

	UpdateChildren(dt);
	//updateChildren(Tree(list, values), dt) 
}
	void Model::updatePath(float dt){
	//Translate Along Path
	//==============================================================================
	bool makeMove = true;
	glm::vec3 vTargetWaypoint	= mPath->GetWaypoint(mTargetWaypoint);						//current target waypoint
		
	//Compair distance with distance to go
	float step = mSpeed * dt;
	float distToGo = glm::distance(mPosition, vTargetWaypoint);	
	if (distToGo < step){																
		mPosition = vTargetWaypoint;														//set position to waypoint
		dt =- distToGo/mSpeed;																//recalculate dt with new position
			
		//are there any more waypoints? continue on the way to the next waypoint
		mTargetWaypoint++;																	//change target
		mTargetWaypoint %= mPath->GetWaypointSize();										//prevent overflow

		vTargetWaypoint	= mPath->GetWaypoint(mTargetWaypoint);								//current target waypoint
		// else makeMove = false;
	}

	if(makeMove){
		glm::vec3 moveDirection		= glm::normalize(vTargetWaypoint - mPosition);			//step direction toward waypoint, from model
		step = mSpeed * dt;	
		mPosition += step*moveDirection;													//step toward direction
	}
}
	void Model::updateSpline(float dt){
	//Translate Along Spline	 @TODO - Animate along the spline
		//==============================================================================
		mSplineParameterT = mSpline->Travel(mSplineParameterT, dt*mSpeed);
		
		//every lap reset T-param
		int cPSize = mSpline->GetControlPointSize();
		if(floor(mSplineParameterT) == cPSize) 
			mSplineParameterT -= cPSize;
		
		mPosition = mSpline->GetPosition(mSplineParameterT);
}

//Draw------------------------------------------
void Model::Draw(){}


glm::mat4 Model::GetWorldMatrix(){
	mat4 worldMatrix(1.0f);

	mat4 t = glm::translate(mat4(1.0f), mPosition);
	mat4 r = glm::rotate(mat4(1.0f), mRotationAngleInDegrees, mRotationAxis);
	mat4 s = glm::scale(mat4(1.0f), mScaling);
	worldMatrix = t * r * s;
	
	if(HasParent()){//relative to parent
		return Parent()->GetWorldMatrix() * worldMatrix;
	} else //relative to world
		return worldMatrix;
}

//#########################################################

//						Orientation

/////#####################################################
void Model::SetPosition(glm::vec3 position){
	mPosition = position;
}
void Model::SetScaling(glm::vec3 scaling){
	mScaling = scaling;
}
void Model::SetRotation(glm::vec3 axis, float angleDegrees){
	mRotationAxis = axis;
	mRotationAngleInDegrees = angleDegrees;
}


//#########################################################

//						Children

/////#####################################################
bool Model::HasParent(){
	if (mParent != nullptr)
		return true;
	return false;
}
Model* Model::Parent(){
	return mParent;
}
//----------------------------------------
void	Model::AddChild(Model* m){
	m->SetParent(this);
	mChildren.push_back(m);
}	
Model*	Model::RemoveChild(Model* m){ return nullptr;}
//----------------------------------------
void	Model::UpdateChildren(float dt){
	int count = GetChildCount();
	if (count > 0){
		for(int i=0; i<count; i++){
			mChildren[i]->Update(dt);
		}
	}
}
void	Model::DrawChildren(){
	int count = GetChildCount();
	if (count > 0){
		for(int i=0; i<count; i++){
			mChildren[i]->Draw();
		}
	}
}
//----------------------------------------
void Model::SetParent(Model* m){
	mParent = m;
}



//Physics ------------------------------------------------
void Model::SetSpeed(float spd){
    mSpeed = spd;
}
float Model::GetSpeed(){return mSpeed;}
//force
//acceleration

