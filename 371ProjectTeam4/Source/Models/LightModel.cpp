//
//

#include "LightModel.h"
#include "../Renderer.h"

#include <vector>
#include <GL/glew.h>// Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h>
using namespace glm;

LightModel::LightModel(glm::vec3 pos, glm::vec3 color) : Model(){
	mLastColor = color;
	mPosition = pos;
	
	//Lights
	World* w = World::GetInstance();
	//mLightIndex = w->AddLight(glm::vec4(mPosition, 0), color);

	directional = 1;
	intensities = vec3(0);
	attenuation = 0.5f;
	ambientCoefficient = 0.5f;
	coneAngle = 0.5f;
	coneDirection = vec3(0,0,1);
}

LightModel::~LightModel(){}

void LightModel::Update(float dt){
	Model::Update(dt);

	
	World* w = World::GetInstance();
	vec3 pos = vec3(GetWorldMatrix()*vec4(mPosition,1));
	//w->UpdateLight(mLightIndex, glm::vec4(pos,1), mLastColor);
}

void LightModel::Draw(){}

bool LightModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	} else {
		return LightModel::ParseLine(token);
	}
}
