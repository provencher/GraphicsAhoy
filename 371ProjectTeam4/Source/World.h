//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "ParsingHelper.h"
#include <vector>
#include "light.h"
#include <GLM/glm.hpp>

class Camera;
class Model;
class Path;
class BSpline;

class World
{
public:
	World();
	~World();
	
    static World* GetInstance();

	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);
    void LoadCameras();

    Path* FindPath(ci_string pathName);
    BSpline* FindSpline(ci_string pathName);
    BSpline* FindSplineByIndex(unsigned int index);
    Model* FindModelByIndex(unsigned int index);
	
	Light* light;	
	glm::vec3 camPos;

private:
    static World* instance;

	std::vector<Model*> mModel;
    std::vector<Path*> mPath;
    std::vector<BSpline*> mSpline;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;
	Camera* GetCamera();

	// Material Coefficients
	float ka;
	float kd;
	float ks;
	float n;
};
