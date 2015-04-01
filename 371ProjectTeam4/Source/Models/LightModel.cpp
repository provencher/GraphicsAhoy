//
//

#include "LightModel.h"
#include "../Renderer.h"
#include "../World.h"
#include <vector>
#include <GL/glew.h>// Include GLEW - OpenGL Extension Wrangler

using namespace glm;

LightModel::LightModel(vec3 size) : Model()
{
	// Create Vertex Buffer for all the verices of the Layer
	vec3 halfSize = size * 0.5f;

	//initialize rotation speed;
	rotationSpeed = 0.0f;

	World* w = World::GetInstance();
	
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

	w->gLights
	*/

}

LightModel::~LightModel()
{

}


void LightModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning Layers!
	// That will only work if your world transform is correct...

	if(rotationSpeed != 0.0f){
		mRotationAngleInDegrees += rotationSpeed * dt;
	}
	Model::Update(dt);
}

void LightModel::Draw(){
	DrawChildren();
	
	
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


