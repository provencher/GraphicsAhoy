//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "Model.h"
#include "../Path.h"
#include "../BSpline.h"
#include "../light.h"
#include "../World.h"

class LightModel : public Model {
public:
	LightModel(glm::vec3 pos= glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	~LightModel();
	void Update(float dt);
	void Draw();


	void SetIsDirectional(bool w){directional = (int)directional;}
	void SetIntensities(glm::vec3 color){intensities = color;}
	void SetAttenuation(float c){attenuation = c;}
	void SetAmbientCoefficient(float c){ambientCoefficient = c;}
	void SetConeAngle(float ang){coneAngle = ang;}
	void SetConeDirection(glm::vec3 dir){coneDirection = dir;}

	int GetIsDirectional()		{return directional;}
	glm::vec3 GetIntensities()	{return intensities;}
	float GetAttenuation()		{return attenuation;}
	float GetAmbientCoefficient(){return ambientCoefficient;}
	float GetConeAngle()		{return coneAngle;}
	glm::vec3 GetConeDirection(){return coneDirection;}

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:

	World* mWorld;
	int mLightIndex;
	glm::vec3 mLastColor;
	std::vector<Light*> mLight;

	
	int directional;
	glm::vec3 intensities;
	float attenuation;
	float ambientCoefficient;
	float coneAngle;
	glm::vec3 coneDirection;
};
