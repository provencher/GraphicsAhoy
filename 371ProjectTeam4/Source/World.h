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
#include <glm/gtc/type_ptr.hpp>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <queue>
#include <vector>

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
	void DrawShadow();

	void LoadScene(const char * scene_path);
    void LoadCameras();

    Path* FindPath(ci_string pathName);
    BSpline* FindSpline(ci_string pathName);
    BSpline* FindSplineByIndex(unsigned int index);
    Model* FindModelByIndex(unsigned int index);

	std::vector<Model*>* GetModels() { return &mModel; }
	
	//Continuous world generation functions
	void generateWorldSection(Model* character);
	void generateObstacles(Model* groundPlate);
	void setGroundQueue(std::vector<std::queue<ci_string>> queue);
	std::vector<std::queue<ci_string>> getGroundQueue();
	void checkPositionOfPlayer(Model* character);

	int numPlates;

	
	void setGround(Model* ground);
	Model* getGround();

	//Return the player model - i.e. the plane
	Model* getPlayerModel();
	glm::vec3 camPos;

	inline void SetLightPostion();
	/*
	glm::vec4 position;
		glm::vec3 intensities; //a.k.a. the color of the light
	*/
	int AddLight(glm::vec4 pos, glm::vec3 color);
	void RemoveLight(int index);
	void UpdateLight(int index, glm::vec4 pos, glm::vec3 color);
	void UpdateLight(int index, glm::vec4 pos, glm::vec3 color, float attenuation, float ambientCoefficient);
	void UpdateLight(int index, glm::vec4 pos, glm::vec3 color, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection);
	
	

	struct Light {
		glm::vec4 position;
		glm::vec3 intensities; //a.k.a. the color of the light
		float attenuation;
		float ambientCoefficient;
		float coneAngle;
		glm::vec3 coneDirection;
	};

	std::vector<Light>* gLights;
	Camera* GetCamera();

	glm::mat4 depthProjectionMatrix;
	glm::mat4 lightVP;
	glm::mat4 depthMVP;

	glm::mat4 biasMatrix;

private:
    static World* instance;

	std::vector<Model*> mModel;
    std::vector<Path*> mPath;
    std::vector<BSpline*> mSpline;
	std::vector<Camera*> mCamera;
	unsigned int mCurrentCamera;
	
	Model* playerModel;

	glm::mat4 projMat;

	Camera* altCamera;
	int width;
	int height;

	//Stores the ground separarely to store and access it more easily
	std::vector<std::queue<ci_string>> groundQueue;
	Model* ground;


	// Material Coefficients
	float ka;
	float kd;
	float ks;
	float n;
};
