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
#include "Texture.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <map>

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
	void DrawObjects();
	void DrawPathLines();
	void DrawShadow();

	void LoadScene(const char * scene_path);
    void LoadCameras();

    Path* FindPath(ci_string pathName);
    BSpline* FindSpline(ci_string pathName);
    BSpline* FindSplineByIndex(unsigned int index);
    Model* FindModelByIndex(unsigned int index);	
		
	glm::vec3 camPos;

	struct Light {
		glm::vec4 position;
		glm::vec3 intensities; //a.k.a. the color of the light
		float attenuation;
		float ambientCoefficient;
		float coneAngle;
		glm::vec3 coneDirection;
		//glm::mat4 m_shadowInfo;
	};

	std::vector<Light>* gLights;
	inline void SetTexture(const std::string& name, Texture* value) { m_textureMap[name] = value; }
	inline Texture* GetTexture(std::string name) { return m_textureMap[name]; }

	glm::mat4 lightVP;
	

private:
    static World* instance;

	std::vector<Model*> mModel;
    std::vector<Path*> mPath;
    std::vector<BSpline*> mSpline;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;
	Camera* GetCamera();
	Camera* altCamera;

	std::map<std::string, unsigned int> m_samplerMap;
	std::map<std::string, Texture*> m_textureMap;
	

	// Material Coefficients
	float ka;
	float kd;
	float ks;
	float n;	

};
