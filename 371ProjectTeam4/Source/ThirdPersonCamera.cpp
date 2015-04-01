//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 28/1/15
// Updated by Jordan Rutty
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//
#include "ThirdPersonCamera.h"
#include "EventManager.h"
#include "World.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>
#include <math.h>
using namespace glm;
//####$%^%$#@!$%^&*%$#@$%^&*(^%$#@$%^&*(&^%$#$%^&*()*&^%$#%^&*(#$%^&*^%$#%^&*(^%$

//			Flying Player Movement

//####$%^%$#@!$%^&*%$#@$%^&*(^%$#@$%^&*(&^%$#$%^&*()*&^%$#%^&*(#$%^&*^%$#%^&*(^%$
void my_int_func(int x) //Dynamic function being called
{
    printf( "%d\n", x );
}
/////////////////////////////////////////////////////////
ThirdPersonCamera::ThirdPersonCamera(Model* targetModel)
    : Camera(), mTargetModel(targetModel), mHorizontalAngle(0.0f), mVerticalAngle(0.0f), mRadius(10.0f){
    assert(mTargetModel != nullptr);

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
}
ThirdPersonCamera::~ThirdPersonCamera(){
}
void ThirdPersonCamera::SetCameraRadius(float r){
	mRadius = r;
}
float ThirdPersonCamera::GetCameraRadius(){
	return mRadius;
}
void ThirdPersonCamera::Update(float dt)
{

	    // @TODO
	//*///////////////////////////////////////////////////////////////
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
	
    // 2 - Clamp vertical angle to [-85, 85] degrees
	if(		mVerticalAngle/pif*180 < -85)	mVerticalAngle = -85*(pif/180);
	else if(mVerticalAngle/pif*180 > 85)	mVerticalAngle = 85*(pif/180);
    // 3 - Wrap Horizontal angle within [-180, 180] degrees
	if(		mHorizontalAngle > pif)			mHorizontalAngle -= 2*pif;
	else if(mHorizontalAngle < -pif)		mHorizontalAngle += 2*pif;


	//Calculate Camera Base Vectors --------------------------
	updateCameraLookAt();
	//*///////////////////////////////////////////////////////////////
	

	 // @TODO
	UpdateTargeModel(dt);

	// @TODO
	//*///////////////////////////////////////////////////////////////
    // Align target model with the horizontal angle
	mTargetModel->SetRotation(mTargetModel->GetRotationAxis(), (mHorizontalAngle/pif*180));
	mPosition = mTargetModel->GetPosition() - mLookAt*mRadius;
	/*idea
	mat4 Model transform = new transformModel()->SetRotation;
	transform = model
	
	//*/
	//lock camera above ground
	//if(mPosition.y <= 0.1f)	mPosition.y = 0.1f;
	//*///////////////////////////////////////////////////////////////
   


	

}
void ThirdPersonCamera::updateCameraLookAt(){
}
glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
    return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
}
//////////////////////////////////////////////////////////

//Update Target Model

void ThirdPersonCamera::UpdateTargeModel(float dt){
		 //*//Move Target Model ////////////////////////////////////////////
	//direction ------------------------------------------------------
	
		

	/* some crazy stuff with adding an extra transform to model
	glm::mat4 m = glm::mat4(
		cos(mVerticalAngle) * sin(mHorizontalAngle), 0, 0, 0,
		0, sin(mVerticalAngle), 0, 0,
		0,0,cos(mVerticalAngle) * cos(mHorizontalAngle),0,
		0,0,0,1);	
	//glm::mat4 m = vec4(mLookAt,1)*glm::mat4(1.0f);
	mTargetModel->transform = m;
	*/

	SetLookAt(mLookAt);
	SetRight(mRight);
	SetUp(mUp);


	///////////////////////////////////////////////////

	//POSITION



	glm::vec3 direction = mLookAt;//mVelocity;
	direction.y = 0.0f; // override to keep movement on plane
	direction = glm::normalize(direction);


	glm::vec3 tempUp = glm::normalize(glm::cross(vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f)));
	
	
	int turn = 0;
	int verticalTilt = 0;
	float tiltspeed = 20.0f;

	//vec3 v3MaxAngles = vec3(0,0,0);
	float maxZTilt = 45;
	float maxYTilt = 90;
	float maxXTilt = 90;

	//normalTilt
	float normalTiltZ = 0;
	float normalTiltX = 0;
	float normalTiltY = 0;
	
	// Movement  -----------------------------------------------------
	glm::vec3 movementDir = glm::vec3(0,0,0);
    if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS){ //Forward
		movementDir += direction;
	} 

	// Down
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS){ //Back
		//Point down -------------------------------
		movementDir -= direction;
	} 

	//===========================================================================
	//Roll Angle
	if(1){
		// Left
		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS){ //Left
			mTargetModel->mRotationAngleZ-=0.2*tiltspeed;

			//Limit Right roll
			if(mTargetModel->mRotationAngleZ < -maxZTilt) 
				mTargetModel->mRotationAngleZ = -maxZTilt;

			//left
			turn--;
			//mHorizontalAngle --;
		}

		// Right
		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS){ //Right
			//Go right
			movementDir += mRight;
			mTargetModel->mRotationAngleZ+=0.2*tiltspeed;
			//Limit Right roll
			if(mTargetModel->mRotationAngleZ > maxZTilt) 
				mTargetModel->mRotationAngleZ = maxZTilt;

			//right
			turn++;
			//mHorizontalAngle++;
		}
	}

	//===========================================================================
	// Right
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS){ 
		movementDir += mRight;

	}
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS){ 
		movementDir -= mRight;
		//-------------------------------------------
	}


	//Move up Move down
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

	//mTargetModel->mRotationAngleZ
	//*
	if(turn == 0){
		//Correct roll - 
		if(mTargetModel->mRotationAngleZ < normalTiltZ)
			mTargetModel->mRotationAngleZ += 0.2*tiltspeed;

		//Correct roll +
		if(mTargetModel->mRotationAngleZ > -normalTiltZ)
			mTargetModel->mRotationAngleZ -= 0.2*tiltspeed;
		//mTargetModel->mRotationAngleZ = 0.0f;
		//mTargetModel->mRotationAngleY = 0.0f;
	}

	if(verticalTilt == 0){
		//Correct roll - 
		if(mTargetModel->mRotationAngleX < normalTiltX)
			mTargetModel->mRotationAngleX += 0.2*tiltspeed;

		//Correct roll +
		if(mTargetModel->mRotationAngleX > -normalTiltX)
			mTargetModel->mRotationAngleX -= 0.2*tiltspeed;
		//mTargetModel->mRotationAngleZ = 0.0f;
		//mTargetModel->mRotationAngleY = 0.0f;
	}
	//*/

	
	
	//distance -------------------------------------------------------
	float dist = dt*mTargetModel->GetSpeed();
	movementDir = glm::normalize(movementDir)*dist;
	//position -------------------------------------------------------
	glm::vec3 pos = mTargetModel->GetPosition();
	if(glm::length(movementDir) > 0)
		pos = pos + movementDir;
	mTargetModel->SetPosition(pos);
	//*///////////////////////////////////////////////////////////////


	
	//////////////////////////////////////////////////

	// LOOK AT

	mLookAt = glm::vec3(
		cos(mVerticalAngle) * sin(mHorizontalAngle), 
		sin(mVerticalAngle),
		cos(mVerticalAngle) * cos(mHorizontalAngle)
	);
	mRight = glm::normalize(glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f)));
    mUp = glm::cross(mRight, mLookAt);
}

//////////////////////////////////////////////////////////


//####$%^%$#@!$%^&*%$#@$%^&*(^%$#@$%^&*(&^%$#$%^&*()*&^%$#%^&*(#$%^&*^%$#%^&*(^%$
//create dev class with debug funcs
	/*Dump Vector to console
	//dev::showVector(glm::vec3 v);
	glm::vec3 v = movementDir;
	//----------------------------
	float vars[3] = {v.x, v.y, v.z};
	for(int i=0; i<3; i++)
		std::cout<<vars[i]<<" ";
	std::cout<<"\n";
	/*///----------------------------
    