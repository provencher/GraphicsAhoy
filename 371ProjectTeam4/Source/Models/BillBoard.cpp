//
// COMP 371 Assignment Framework
//
// Created by Jordan Rutty on 2015-03-17
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "BillBoard.h"
#include "PropellerModel.h"
#include "CubeModel.h"
#include "SphereModel.h"
#include "TorrentModel.h"
#include "../Renderer.h"
#include "../World.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

BillBoard::BillBoard(vec3 size) : GroupModel(){

	//*Create Board
	if(1){
		float s = 2.0f;
		CubeModel* bodypart= new CubeModel();
		bodypart->SetScaling(vec3(0.1f, s, s));
		bodypart->SetPosition(glm::vec3(0.4f,	-0.1f,	-1.4f));
		bodypart->SetRotation(glm::vec3(0.0f,	1.0f,	0.0f), -5.0f);
		//bodypart->SetLookat();
		bodypart->GetRotationAxis();
		//bodypart->GetLookAt();
		AddChild(bodypart);
	}
	//*/

	mLookAtCamera = nullptr;
	glm::vec3 defaultDir = glm::vec3(0,0,1);							//front of the model
	
	mLookAt = defaultDir;
	mRight = glm::normalize(glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f)));
    mUp = glm::normalize(glm::cross(mRight, mLookAt));

	//initialize rotation ---------------------------
	mHorizontalAngle = atan(-mLookAt.z/mLookAt.x); 
	mVerticalAngle = 0.0f;//asin(mLookAt.y);

	
	//SetRotation(mLookAtTarget->GetRotationAxis(), (mHorizontalAngle/3.14159265358979323846f*180));
	

}

BillBoard::~BillBoard(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void BillBoard::Update(float dt){
	World* w = World::GetInstance();
    

	//mHorizontalAngle -= mouseSpeed * dx;
	//mVerticalAngle   -= mouseSpeed * dy;
	//-------------------------------------------------------


	
	//Apply Limits Camera Angles ----------------------------
	float pif = 3.14159265358979323846f;	//PI
    // 2 - Clamp vertical angle to [-85, 85] degrees
	if(		mVerticalAngle/pif*180 < -85)	mVerticalAngle = -85*(pif/180);
	else if(mVerticalAngle/pif*180 > 85)	mVerticalAngle = 85*(pif/180);
    // 3 - Wrap Horizontal angle within [-180, 180] degrees
	if(		mHorizontalAngle > pif)			mHorizontalAngle -= 2*pif;
	else if(mHorizontalAngle < -pif)		mHorizontalAngle += 2*pif;

	/*
	if(mLookAtCamera != nullptr){
		vec3 campos = mLookAtCamera->getCamPos;
		vec3 pos = GetPosition();
		mLookAt = campos - pos;

		
		mRight = glm::normalize(glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f)));
		mUp = glm::cross(mRight, mLookAt);
		
	}
	/*/

	//w->GetCamera();
	GroupModel::Update(dt);
}

void BillBoard::Draw(){
	GroupModel::Draw();
}

bool BillBoard::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	} else {
		return Model::ParseLine(token);
	}
}


//Camera
void BillBoard::SetLookAtCamera(Camera* c){
	mLookAtCamera = c;
}

glm::mat4 BillBoard::GetWorldMatrix(){
	mat4 worldMatrix(1.0f);


	if(mLookAtCamera != nullptr){
		//Please dont mind the mess, jsut hacking things together 

		//------------------------------------------------------------
		vec3 lookAt = normalize(mLookAtCamera->getCamPos()-mPosition);
		float A = asin(lookAt.y)/3.14159265358979323846f*180;
		float B = atan(-lookAt.z/lookAt.x)/3.14159265358979323846f*180;
		//std::cout << mLookAtCamera->getCamPos().x << "\n";


		vec3 camPos = mLookAtCamera->getCamPos();

		vec3 rotaxis = normalize(cross(vec3(0,0,1), lookAt));
		float ang = acos(dot(normalize(vec3(0,0,1)), normalize(lookAt)));

		mat4 rX = glm::rotate(mat4(1.0f), ang, rotaxis);


		mat4 t = glm::translate(mat4(1.0f), mPosition);
		mat4 r = glm::rotate(mat4(1.0f), A, vec3(1,0,1));
		mat4 r2 = glm::rotate(r, B, vec3(0,1,0));

		//mHorizontalAngle = atan(-mLookAt.z/mLookAt.x); 
		//mVerticalAngle = asin(mLookAt.y);
		mLookAt = glm::vec3(
			cos(mVerticalAngle) * sin(mHorizontalAngle), 
			sin(mVerticalAngle),
			cos(mVerticalAngle) * cos(mHorizontalAngle)
		);
		//camPos-mPosition

		//mat4 rotate = glm::rotate(mat4(1.0f), B, vec3(0,1,0));

		//------------------------------------------------------------
		mat4 s = glm::scale(mat4(1.0f), mScaling);
		worldMatrix = t * r2 * s;
		//worldMatrix = inverse(glm::lookAt(mPosition, mPosition + lookAt, vec3(0,1,0))) * t * r * s;
		//mPosition
		//mat4 camw  = glm::lookAt(mLookAtCamera->GetPosition(), mLookAtCamera->GetPosition() + mLookAtCamera->GetLookAt(), mLookAtCamera->GetUp());
		//worldMatrix = t * r * s;
		/*
		mRight = glm::normalize(glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f)));
		mUp = glm::cross(mRight, mLookAt);
		//*/
		//return glm::lookAt(mPosition, mPosition + mLookAt, mUp);
		return worldMatrix;
	} else {
		//mat4 wm = Model::GetWorldMatrix();
		mat4 t = glm::translate(mat4(1.0f), mPosition);
		mat4 r = glm::rotate(mat4(1.0f), mRotationAngleInDegrees, mRotationAxis);
		mat4 s = glm::scale(mat4(1.0f), mScaling);
		worldMatrix = t * r * s;
	
		if(HasParent()){//relative to parent
			return Parent()->GetWorldMatrix() * worldMatrix;
		} else //relative to world
			return worldMatrix;
	}
}
