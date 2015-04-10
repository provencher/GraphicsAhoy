//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
// Updated by Jordan Rutty
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//
#include "PlayerCamera.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "Models/particleEmitter.h"
#include "Models/LightModel.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <math.h>
using namespace glm;
//####$%^%$#@!$%^&*%$#@$%^&*(^%$#@$%^&*(&^%$#$%^&*()*&^%$#%^&*(#$%^&*^%$#%^&*(^%$

//			Flying Player Movement

//####$%^%$#@!$%^&*%$#@$%^&*(^%$#@$%^&*(&^%$#$%^&*()*&^%$#%^&*(#$%^&*^%$#%^&*(^%$
/////////////////////////////////////////////////////////
PlayerCamera::PlayerCamera(Model* targetModel)
    : Camera(),  mHorizontalAngle(0.0f), mVerticalAngle(0.0f), mRadius(10.0f){
    assert(mTargetModel != nullptr);

	mTargetModel = targetModel;
	//Orient ---------------------------------------
	glm::vec3 defaultDir = glm::vec3(0,0,1);							//front of the model
	//mTargetModel->SetRotation(mTargetModel->GetRotationAxis(), 45);	//test initial rotation

	//Calculate Bases ------------------------------
	mLookAt = glm::rotate(defaultDir, mTargetModel->GetRotationAngle(), mTargetModel->GetRotationAxis());
	mRight = glm::normalize(glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f)));
    mUp = glm::normalize(glm::cross(mRight, mLookAt));

	
	//initialize rotation ---------------------------
	mHorizontalAngle = atan(-mLookAt.z/mLookAt.x); 
	//mVerticalAngle = asin(mLookAt.y);
	

	//position and orient ---------------------------
	mPosition = mTargetModel->GetPosition() - mLookAt*mRadius;
	mTargetModel->SetRotation(mTargetModel->GetRotationAxis(), (mHorizontalAngle/3.14159265358979323846f*180));

	//defailt move direction
	mVelocity = mLookAt;

	
	
	//*Add Spotlight
	if(1){
		mSpotLight = new LightModel();
		mSpotLight->SetPosition(vec3(0,0.0f,1.5f));
		mSpotLight->SetIntensities(5.0f*vec3(1,0.5f,0.1f));
		mSpotLight->SetIsDirectional(1);
		mSpotLight->SetAttenuation(0.5f);
		mSpotLight->SetAmbientCoefficient(0.0f);
		mSpotLight->SetConeAngle(35);
		mSpotLight->SetConeDirection(glm::rotate(vec3(0,0,1), 20.0f, vec3(1,0,0)));	//forward facing spotlight
		mTargetModel->AddChild("SpotLight", mSpotLight);
	}//*/

}
PlayerCamera::~PlayerCamera(){
}
void PlayerCamera::SetCameraRadius(float r){
	mRadius = r;
}
float PlayerCamera::GetCameraRadius(){
	return mRadius;
}
void PlayerCamera::Update(float dt)
{

	//Get Mouse
	float dx, dy;
	dx = EventManager::GetMouseDX();
	dy = EventManager::GetMouseMotionY();
	//-----------------------------------
	EventManager::DisableMouseCursor();
	
	float pif = 3.14159265358979323846f;	//PI
	//*
	//------------------------------------
	// 1 - Map Mouse motion to Spherical Angles
	float mouseSpeed = 0.005f;
	mHorizontalAngle -= mouseSpeed * dx;
	mVerticalAngle   -= mouseSpeed * dy;
	//-------------------------------------------------------


	
	//Apply Limits Camera Angles ----------------------------
	
    // 2 - Clamp vertical angle to min and max degrees
	float minAngle = -35;
	float maxAngle = 40;
	if(		mVerticalAngle/pif*180 < minAngle)	mVerticalAngle = minAngle*(pif/180);
	else if(mVerticalAngle/pif*180 > maxAngle)	mVerticalAngle = maxAngle*(pif/180);
    // 3 - Wrap Horizontal angle within [-180, 180] degrees
	if(		mHorizontalAngle > pif)			mHorizontalAngle -= 2*pif;
	else if(mHorizontalAngle < -pif)		mHorizontalAngle += 2*pif;


	updateCameraLookAt();

	UpdateTargeModel(dt);

	// @TODO
	//*///////////////////////////////////////////////////////////////
    // Align target model with the horizontal angle
	mTargetModel->SetRotation(mTargetModel->GetRotationAxis(), (mHorizontalAngle/pif*180));
	mPosition = mTargetModel->GetPosition() - mLookAt*mRadius;


	//mSpotLight->SetConeDirection(mLookAt); // spotlight strait ahead
	glm::vec3 spotLightDir = -normalize(mLookAt);
	spotLightDir.y=0;
	//spotLightDir = glm::rotate(spotLightDir, 20.0f, vec3(1,0,0))
	mSpotLight->SetConeDirection(spotLightDir); // spotlight tilted
	//mSpotLight->SetPosition(vec3(mTargetModel->GetWorldMatrix()*vec4(mPosition, 1)));
	

	//lock camera above ground
	if(mPosition.y <= 0.1f)	
		mPosition.y = 0.1f;

}
void PlayerCamera::updateCameraLookAt(){
}
glm::mat4 PlayerCamera::GetViewMatrix() const
{
    return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}
//////////////////////////////////////////////////////////

//Update Target Model

void PlayerCamera::UpdateTargeModel(float dt){
		

	///////////////////////////////////////////////////
	//POSITION

	glm::vec3 direction = mLookAt;//mVelocity;
	direction.y = 0.0f; // override to keep movement on plane
	direction = glm::normalize(direction);

	// Movement  -----------------------------------------------------
	glm::vec3 movementDir = glm::vec3(0,0,0);
    if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS){ //Forward
		movementDir += direction;
	} 

	// Down
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS){ //Back
		movementDir -= direction;
	} 





	////////////////////////////////////////////////////////////////////////////////////////
	//Plane Dynamics - Added by Jordan
	glm::vec3 tempUp = glm::normalize(glm::cross(vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)));
	
	// Settings -------------------------------------------------------
	int turn = 0;				//turn direction - for left + for right
	int verticalTilt = 0;		//- for up + for down
	float tiltspeed = 7.0f;	
	float turnSpeed = 0.1f;	//speed of turning

	//vec3 v3MaxAngles = vec3(0,0,0);
	float maxZTilt = 45;
	float maxYTilt = 45;
	float maxXTilt = 20;

	//normalTilt
	float normalTiltZ = 0;
	float normalTiltX = 0;
	float normalTiltY = 0;

	//Roll Angle --------------------------------------------------------------
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS){ //Left
		turn--;
		if(mTargetModel->mRotationAngleZ+tiltspeed > normalTiltZ)
			mTargetModel->mRotationAngleZ-=2*tiltspeed;		//tilt faster if already leaning in other direction 
		else 
			mTargetModel->mRotationAngleZ-=tiltspeed;		//tilt normal speed
		if(mTargetModel->mRotationAngleZ < -maxZTilt)		//Limit left roll
			mTargetModel->mRotationAngleZ = -maxZTilt;

		mHorizontalAngle += turnSpeed;//<-aron contribute
	}
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS){ //Right
		//Go right
		//tilt right
		turn++;
		if(mTargetModel->mRotationAngleZ-tiltspeed < normalTiltZ)
			mTargetModel->mRotationAngleZ+=2*tiltspeed;		//tilt faster if already leaning in other direction 
		else 
			mTargetModel->mRotationAngleZ+=tiltspeed;		//tilt normal speed
		if(mTargetModel->mRotationAngleZ > maxZTilt)		//Limit right roll
			mTargetModel->mRotationAngleZ = maxZTilt;

		mHorizontalAngle -= turnSpeed;//<-aron contribute
	}
	
	/*// HOROZONTAL MOVEMENT -------------------------------------------------
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS){ 
		movementDir += mRight;
	}
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS){ 
		movementDir -= mRight;
	}//*/

	//Control Flap controls when turning --------------------------------------------------------
	float flapFactorH = mTargetModel->mRotationAngleZ/maxZTilt;
	float flapFactorV = mTargetModel->mRotationAngleX/maxXTilt;
	float maxFlapAngle = 30;
	//Left Flap
	Model* leftFlap = mTargetModel->child["wing"]->child["left"]->child["flap"];
	leftFlap->SetRotation(leftFlap->GetRotationAxis(), -maxFlapAngle*flapFactorH -maxFlapAngle*flapFactorV);
	//Right Flap
	Model* rightFlap = mTargetModel->child["wing"]->child["right"]->child["flap"];
	rightFlap->SetRotation(rightFlap->GetRotationAxis(), maxFlapAngle*flapFactorH -maxFlapAngle*flapFactorV);
	
	//Move up Move down ------------------------------------------------------------------------
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_SPACE ) == GLFW_PRESS){ //Up
		movementDir += tempUp;
		mTargetModel->mRotationAngleX-=0.2*tiltspeed;
			//Limit Right roll
			if(mTargetModel->mRotationAngleX < -maxXTilt) 
				mTargetModel->mRotationAngleX = -maxXTilt;
		verticalTilt--;
	}
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_RIGHT_SHIFT ) == GLFW_PRESS ||
		glfwGetKey(EventManager::GetWindow(), GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){ //Down
		movementDir -= tempUp;
		mTargetModel->mRotationAngleX+=0.2*tiltspeed;
			//Limit Right roll
			if(mTargetModel->mRotationAngleX > maxXTilt) 
				mTargetModel->mRotationAngleX = maxXTilt;
		verticalTilt++;

	}

	//Correct roll ----------------------------------------------------------------------------
	if(turn == 0){
		if(mTargetModel->mRotationAngleZ < normalTiltZ)
			if(mTargetModel->mRotationAngleZ+tiltspeed > normalTiltZ)	//if less then increment set to normal
				mTargetModel->mRotationAngleZ = normalTiltZ;
			else
				mTargetModel->mRotationAngleZ += tiltspeed;

		if(mTargetModel->mRotationAngleZ > -normalTiltZ)
			if(mTargetModel->mRotationAngleZ-tiltspeed < normalTiltZ)	
				mTargetModel->mRotationAngleZ = normalTiltZ;
			else
				mTargetModel->mRotationAngleZ -= tiltspeed;
	}

	//Correct tilt ----------------------------------------------------------------------------
	if(verticalTilt == 0){
		if(mTargetModel->mRotationAngleX < normalTiltX)
			if(mTargetModel->mRotationAngleX+0.2*tiltspeed > normalTiltX)
				mTargetModel->mRotationAngleX = normalTiltX;
			else
				mTargetModel->mRotationAngleX += 0.2*tiltspeed;

		if(mTargetModel->mRotationAngleX > -normalTiltX)
			if(mTargetModel->mRotationAngleX-0.2*tiltspeed < normalTiltX)
				mTargetModel->mRotationAngleX = normalTiltX;
			else
				mTargetModel->mRotationAngleX -= 0.2*tiltspeed;
	}
	
	//distance -------------------------------------------------------
	float dist = dt*mTargetModel->GetSpeed();
	movementDir = glm::normalize(movementDir)*dist;
	//position -------------------------------------------------------
	glm::vec3 pos = mTargetModel->GetPosition();
	if(glm::length(movementDir) > 0)
		pos = pos + movementDir;

	if(pos.y < 1.5f) 
		pos.y = 1.5f;	//	min height

	if(pos.y > 28) 
		pos.y = 28;	//	max height
	mTargetModel->SetPosition(pos);
	//*///////////////////////////////////////////////////////////////////////////




	if (ProcessCollisions(*mTargetModel)) // returns whether a collision occurred
	{
		// Payer dies?
	}
	



	//////////////////////////////////////////////////

	// LOOK AT

	mLookAt = glm::vec3(
		cos(mVerticalAngle) * sin(mHorizontalAngle), 
		sin(mVerticalAngle),
		cos(mVerticalAngle) * cos(mHorizontalAngle)
	);

	mRight = glm::normalize(glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f)));
    mUp = glm::cross(mRight, mLookAt);

	//
	if(mTargetModel->HasChild("SpotLight")){
		//LightModel* light = mTargetModel->child["SpotLight"];

	}
	//=====================================================================================
	// Process shooting

	if (glfwGetMouseButton(EventManager::GetWindow(), 0) == GLFW_PRESS)
	{
		Ray shootRay(mTargetModel->GetPosition(), glm::normalize(vec3(mLookAt.x, 0.0f, mLookAt.z)));
		std::vector<Model*>& models = *World::GetInstance()->GetModels();
		std::vector<std::pair<Model*, glm::vec3>> intersectionPoints;
		for (int i = 0, iMax = models.size(); i < iMax; ++i)
		{
			models[i]->Intersect(shootRay, intersectionPoints);
			IntersectChildren_r(shootRay, models[i]->child, intersectionPoints);
		}
		std::pair<Model*, vec3>* closestIntersection = nullptr;
		for (int i = 0, iMax = intersectionPoints.size(); i < iMax; ++i)
		{
			if (intersectionPoints[i].first != mTargetModel && (closestIntersection == nullptr || (distance(intersectionPoints[i].second, shootRay.getOrigin()) < distance(closestIntersection->second, shootRay.getOrigin()))))
			{
				closestIntersection = &(intersectionPoints[i]);
			}
		}
		if (closestIntersection != nullptr)
		{
			ParticleEmitter* pe = new ParticleEmitter(vec3(0,1,0));
			pe->SetPosition(closestIntersection->first->GetPosition());
			pe->SetScaling(vec3(5,1,0.5f));
			closestIntersection->first->Parent()->DeleteChild(closestIntersection->first->GetName());
			std::vector<Model*>& models = *World::GetInstance()->GetModels();
			for (int i = 0, iMax = models.size(); i < iMax; ++i)
			{
				if (models[i]->GetName().compare("Ground") == 0)
				{
					models[i]->AddChild("left-afterburner", pe);
					break;
				}
			}
		}
	}
}

bool PlayerCamera::ProcessCollisions(Model& targetModel)
{
	bool collided = false;
	std::vector<Model*>& models = *World::GetInstance()->GetModels();
	for (int i = 0, iMax = models.size(); i < iMax; ++i)
	{
		if (targetModel.CollideWith(models[i])
			|| CollideChildren_r(targetModel, models[i]->child))
		{
			collided = true;
		}
	}
	return collided;
}

bool PlayerCamera::CollideChildren_r(Model& collider, std::map<ci_string, Model*>& children)
{
	bool collided = false;
	typedef std::map<ci_string, Model*>::iterator it_type;
	for(it_type iterator = children.begin(); iterator != children.end(); iterator++)
	{
		collider.CollideWith(iterator->second);
		if (CollideChildren_r(collider, iterator->second->child))
		{
			collided = true;
		}
	}
	return collided;
}

void PlayerCamera::IntersectChildren_r(const Ray& ray, std::map<ci_string, Model*>& children, std::vector<std::pair<Model*, glm::vec3>>& intersections)
{
	typedef std::map<ci_string, Model*>::iterator it_type;
	for(it_type iterator = children.begin(); iterator != children.end(); iterator++)
	{
		iterator->second->Intersect(ray, intersections);
		IntersectChildren_r(ray, iterator->second->child, intersections);
	}
}