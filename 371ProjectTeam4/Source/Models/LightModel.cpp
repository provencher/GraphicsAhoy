// Created by Jordan Rutty 2014-04-01, with help from Eric Provencher

#include "LightModel.h"
#include "../Renderer.h"
#include <vector>
#include <GL/glew.h>// Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h>
using namespace glm;

//Events ------------------------------------------------------------------------
LightModel::LightModel(glm::vec3 pos, glm::vec3 color) : Model(){
	mColor = color;
	mLastColor = mColor;
	mPosition = pos;
	
	mLightIndex = World::GetInstance()->AddLight(glm::vec4(mPosition, mDir), color);
}
LightModel::~LightModel(){
	//NOTE:: Vector of lights should become a map so the index is independant of sequence position
}
void LightModel::Update(float dt){
	Model::Update(dt);

	World::GetInstance()->UpdateLight(
		mLightIndex,	//key to light
		glm::vec4(vec3(GetWorldMatrix()*vec4(mPosition,mDir)), mDir), //update world position of light, send is directional
		(length(GetScaling())/length(vec3(1)))*mColor, mAttenuation, mAmbientCoefficient, 
		mConeAngle, mConeDirection
	);
}

//Helper ------------------------------------------------------------------------
bool LightModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	} else {
		return LightModel::ParseLine(token);
	}
}


//Setters -----------------------------------------------------------------------
void LightModel::SetIsDirectional(bool w)		{	mDir = (int)w;				}
void LightModel::SetIntensities(glm::vec3 color){	mColor = color;				}
void LightModel::SetAttenuation(float c)		{	mAttenuation = c;			}
void LightModel::SetAmbientCoefficient(float c)	{	mAmbientCoefficient = c;	}
void LightModel::SetConeAngle(float ang)		{	mConeAngle = ang;			}
void LightModel::SetConeDirection(glm::vec3 dir){	mConeDirection = dir;		}

//Getters -----------------------------------------------------------------------
int LightModel::GetIsDirectional()				{	return mDir;				}
glm::vec3 LightModel::GetIntensities()			{	return mColor;				}
float LightModel::GetAttenuation()				{	return mAttenuation;		}
float LightModel::GetAmbientCoefficient()		{	return mAmbientCoefficient;	}
float LightModel::GetConeAngle()				{	return mConeAngle;			}
glm::vec3 LightModel::GetConeDirection()		{	return mConeDirection;		}