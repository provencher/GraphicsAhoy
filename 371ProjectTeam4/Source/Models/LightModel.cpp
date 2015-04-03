//
//

#include "LightModel.h"
#include "../Renderer.h"

#include <vector>
#include <GL/glew.h>// Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h>
using namespace glm;

LightModel::LightModel(glm::vec3 pos, glm::vec3 color) : Model(){
	mColor = color;
	mLastColor = mColor;
	mPosition = pos;
	
	mLightIndex = World::GetInstance()->AddLight(glm::vec4(mPosition, directional), color);
}

LightModel::~LightModel(){
	//NOTE:: Vector of lights should become a map so the index is independant of sequence position
}
bool LightModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	} else {
		return LightModel::ParseLine(token);
	}
}


void LightModel::Update(float dt){
	Model::Update(dt);

	vec3 pos = vec3(GetWorldMatrix()*vec4(mPosition,directional));
	World::GetInstance()->UpdateLight(mLightIndex, glm::vec4(pos,directional), mColor, attenuation, ambientCoefficient, coneAngle, coneDirection);
}

void LightModel::Draw(){}



void LightModel::SetIsDirectional(bool w){			directional = (int)w;	}
void LightModel::SetIntensities(glm::vec3 color){	intensities = color;			}
void LightModel::SetAttenuation(float c){			attenuation = c;				}
void LightModel::SetAmbientCoefficient(float c){	ambientCoefficient = c;			}
void LightModel::SetConeAngle(float ang){			coneAngle = ang;				}
void LightModel::SetConeDirection(glm::vec3 dir){	coneDirection = dir;			}

int LightModel::GetIsDirectional()		{return directional;}
glm::vec3 LightModel::GetIntensities()	{return intensities;}
float LightModel::GetAttenuation()		{return attenuation;}
float LightModel::GetAmbientCoefficient(){return ambientCoefficient;}
float LightModel::GetConeAngle()		{return coneAngle;}
glm::vec3 LightModel::GetConeDirection(){return coneDirection;}