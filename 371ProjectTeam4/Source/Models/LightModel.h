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
	LightModel(glm::vec3 pos, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	~LightModel();
	void Update(float dt);
	void Draw();

	


	void SetIsDirectional(bool w);
	void SetIntensities(glm::vec3 color);
	void SetAttenuation(float c);
	void SetAmbientCoefficient(float c);
	void SetConeAngle(float ang);
	void SetConeDirection(glm::vec3 dir);

	int GetIsDirectional();
	glm::vec3 GetIntensities();
	float GetAttenuation();
	float GetAmbientCoefficient();
	float GetConeAngle();
	glm::vec3 GetConeDirection();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:

	World* mWorld;
	int mLightIndex;
	glm::vec3 mColor;
	glm::vec3 mLastColor;
	std::vector<Light*> mLight;

	glm::vec3 mColorDeviation;

	int directional;
	glm::vec3 intensities;
	float attenuation;
	float ambientCoefficient;
	float coneAngle;
	glm::vec3 coneDirection;
};
