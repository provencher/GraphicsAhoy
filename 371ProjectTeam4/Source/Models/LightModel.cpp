//
//

#include "LightModel.h"
#include "../Renderer.h"
#include "../World.h"
#include <vector>
#include <GL/glew.h>// Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h>
using namespace glm;

LightModel::LightModel(glm::vec3 pos, glm::vec3 color) : Model()
{
	//initialize rotation speed;
	rotationSpeed = 0.0f;
	mLastColor = vec3(0.4f);

	
	////////////////////////////////////////////////////////////
	mPosition = pos;
	//if(Parent() != nullptr){
	
	vec4 tempPos = vec4(mPosition,1);
	//vec4 buildPos = Parent()->GetWorldMatrix()*tempPos;
	vec3 lame = vec3(0);

	
	//Lights
	World* w = World::GetInstance();
	mLightIndex = w->AddLight(glm::vec4(lame, 0), color);
	//std::cout<< "IM HERE#############";
	//}



	/*
	// setup lights
	Light spotlight;
	spotlight.position = glm::vec4(0, 5, 0, 1);
	spotlight.intensities = glm::vec3(0, 1, 2); //strong white light
	spotlight.attenuation = 0.1f;
	spotlight.ambientCoefficient = 0.0f; //no ambient light
	spotlight.coneAngle = 5.0f;
	spotlight.coneDirection = glm::vec3(0, -1, 0);

	Light directionalLight;
	directionalLight.position = glm::vec4(5, 20, 0.6, 0); //w == 0 indications a directional light
	directionalLight.intensities = glm::vec3(1, 1, 1); 
	directionalLight.ambientCoefficient = 0.2f;

	Light light3;
	light3.position = glm::vec4(-5, 5, 15, 0); //w == 0 indications a directional light
	light3.intensities = glm::vec3(0.5, 0.5, 0.5); //weak yellowish light
	light3.ambientCoefficient = 0.06f;

	
	//*/

}

LightModel::~LightModel()
{
	//REMEMBERT
}

void LightModel::Update(float dt)
{
	if(rotationSpeed != 0.0f){
		mRotationAngleInDegrees += rotationSpeed * dt;
	}
	Model::Update(dt);

	if(Parent() != nullptr){
		
		World* w = World::GetInstance();

		vec3 pos = vec3(Parent()->GetWorldMatrix()*vec4(mPosition,1));

		std::cout << "x:"<< pos.x 
			<< "y:"<< pos.y 
			<< "z:"<< pos.z << "\n";

		vec3 col;
		if((int)(glfwGetTime())%2 == 0){
			//*

			if (length(mLastColor) < 1){
				col = vec3(
					(mLastColor.x + (rand()%10)/100.0f),
					(mLastColor.y + (rand()%10)/100.0f),
					(mLastColor.z + (rand()%10)/100.0f)
				);
			} else if(length(mLastColor) > 0) {
				col = vec3(
					(mLastColor.x - (rand()%10)/100.0f),
					(mLastColor.y - (rand()%10)/100.0f),
					(mLastColor.z - (rand()%10)/100.0f)
				);
			}
			

			if(col.x < 0) col.x = 0;
			if(col.y < 0) col.y = 0;
			if(col.z < 0) col.z = 0;

			//*/
			/*
			col = vec3(
			(rand()%10)/10.0f,
			(rand()%10)/10.0f,
			(rand()%10)/10.0f);
			//*/

		} else {
			col = mLastColor;
		}
		
	
		std::cout  << "\n";

		w->UpdateLight(mLightIndex, glm::vec4(pos,0), col);
		mLastColor = col;
		
	}

	// LIGHT ////////////////////////////////////////////////
	//World* w = World::GetInstance();
	//w->UpdateLight(mLightIndex, glm::vec4(mPosition,0), glm::vec3(1,0,0));

	/*
	vec4 tempPos = vec4(mPosition,0);
	vec4 buildPos = Parent()->GetWorldMatrix()*tempPos;
	vec3 lame = vec3(buildPos.x, buildPos.y, buildPos.z);
	*/

	/*
	vec4 tempPos = vec4(0,10,0,0);
	vec4 buildPos = vec4(0);
	buildPos = GetWorldMatrix()*tempPos;
	vec3 lame = vec3(buildPos.x, buildPos.y, buildPos.z);
	//gLights[mLightIndex]->position = vec4(1);
	*/

}

void LightModel::Draw(){
	
	
	
	/*
	// Draw paths
	if(mPath.size() > 0)
		for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
			(*it)->Draw();
	
	/*
	// Draw B-Spline Lines (using the same shader for Path Lines)
	if(mSpline.size() > 0)
		for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
			(*it)->Draw();
	*/
}

bool LightModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	} else {
		return LightModel::ParseLine(token);
	}
}
