//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "BSplineCamera.h"
#include "ThirdPersonCamera.h"

#include "CubeModel.h"
#include "SphereModel.h"
#include "Path.h"
#include "BSpline.h"

#include <GLFW/glfw3.h>
#include "EventManager.h"

#include <string>

using namespace std;
using namespace glm;

World* World::instance;

//=================================================
World::World()
{
    instance = this;
}
World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it){
		delete *it;
	}

	mModel.clear();

    // Paths
    for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it){
		delete *it;
	}
	mPath.clear();

    // Splines
    for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it){
		delete *it;
	}
	mSpline.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it){
		delete *it;
	}
	mCamera.clear();
}
World* World::GetInstance()
{
    return instance;
}

//=================================================
void World::LoadScene(const char * scene_path){
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if(input.fail() ){	 
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while( std::getline( input, item, '[' ) ){
        ci_istringstream iss( item );

		ci_string result;
		if( std::getline( iss, result, ']') ){
			if( result == "cube" ){
				// Load Box --------------
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				mModel.push_back(cube);
			} else if( result == "sphere" ){
				// Load Sphere -----------
                SphereModel* sphere = new SphereModel();
                sphere->Load(iss);
                mModel.push_back(sphere);
            } else if( result == "path" ){
				// Load Path --------------
				Path* path = new Path();
				path->Load(iss);
                mPath.push_back(path);
			} else if( result == "spline" ){
				// Load Spline -------------
				BSpline* path = new BSpline();
				path->Load(iss);
                mSpline.push_back(path);
			} else if ( result.empty() == false && result[0] == '#'){
				// this is a comment line
			} else {
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
	    }
	}
	input.close();
	
	
	/*##############################################################
	
			nesting models
			apply tweens

	################################################################
	//*///






	// Set PATH vertex buffers
	for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it){
		// Draw model
		(*it)->CreateVertexBuffer();
	}

    // Set B-SPLINE vertex buffers
    for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it){
		// Draw model
		(*it)->CreateVertexBuffer();
	}
    
    LoadCameras();
}
void World::LoadCameras()
{
    // Setup Camera ----------------------------------------
    mCamera.push_back(new StaticCamera(
		vec3(3.0f, 5.0f, 5.0f),  
		vec3(0.0f, 0.0f, 0.0f), 
		vec3(0.0f, 1.0f, 0.0f)));//1
    mCamera.push_back(new StaticCamera(
		vec3(3.0f, 30.0f, 5.0f), 
		vec3(0.0f, 0.0f, 0.0f), 
		vec3(0.0f, 1.0f, 0.0f)));//2
    mCamera.push_back(new StaticCamera(
		vec3(0.5f,  0.5f, 5.0f), 
		vec3(0.0f, 0.5f, 0.0f), 
		vec3(0.0f, 1.0f, 0.0f)));//3
    







	// Third Person Cube Character -------------------------
    CubeModel* character = new CubeModel();
	//character->SetRotation(vec3(1,0,0), 90);//change thirs person to accomidate 
	
	
	int sections = 5;
	float scale = 1/(float)sections;


	character->SetScaling(vec3(scale, scale, scale));
	character->SetPosition(vec3(10.0f, 0.5f, 0.0f));
	character->SetSpeed(7.0f);	//Should move to camera
    mModel.push_back(character);
	
	//Build heigharchical cube model (testing)
	//glm::vec3 parentPos = glm::vec3(-20,0.5, 20);
	glm::vec3 parentPos = glm::vec3(0,0,0);
	float ofst = 1.01f;

	

	for(int r=0;r <sections; r++){
		for(int c=0; c<sections; c++){
			for(int d=0; d<sections; d++){
				CubeModel* lcube = new CubeModel();
				(*lcube).SetPosition(glm::vec3(
					parentPos.x + r*ofst,
					parentPos.y + c*ofst,
					parentPos.z + d*ofst
					));
				//lcube->SetRotation(vec3(0.0f,1.0f,0.0f), 270.0f);

				character->AddChild(lcube);
				//mModel.push_back(lcube);
			}
		}
	}


	

    



	// Create Camera
	mCamera.push_back(new ThirdPersonCamera(character)); //4
    


	
    







    // BSpline Camera --------------------------------------
    BSpline* spline = FindSpline("\"RollerCoaster\"");
    if (spline == nullptr)
        spline = FindSplineByIndex(0);
 
    if (spline != nullptr)
        mCamera.push_back(new BSplineCamera(spline , 5.0f)); //5
    
    
    mCurrentCamera = 0;
}


//=================================================
void World::Update(float dt)
{
	// User Inputs
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS){
		mCurrentCamera = 0;
	}else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS){
		if (mCamera.size() > 1){
			mCurrentCamera = 1;
		}
	}else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS){
		if (mCamera.size() > 2){
			mCurrentCamera = 2;
		}
	}else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_4 ) == GLFW_PRESS){
        // Spline camera
		if (mCamera.size() > 3 && mSpline.size() > 0){
			mCurrentCamera = 3;
		}
	}else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_5 ) == GLFW_PRESS){
        // Spline camera
		if (mCamera.size() > 4 && mModel.size() > 0){
			mCurrentCamera = 4;
		}
	}





/*
	// Spacebar to change the shader
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0 ) == GLFW_PRESS){
		Renderer::SetShader(SHADER_SOLID_COLOR);
	}else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9 ) == GLFW_PRESS){
		Renderer::SetShader(SHADER_BLUE);
	}
	//*/
	


	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it){
		(*it)->Update(dt);
	}
}
void World::Draw()
{
	Renderer::BeginFrame();
	
	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform"); 

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Draw models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it){
		// Draw model
		(*it)->Draw();
	}

	// Draw Path Lines
	
	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_PATH_LINES);
	glUseProgram(Renderer::GetShaderProgramID());

	// Send the view projection constants to the shader
	VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	for (vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it){
		// Draw model
		(*it)->Draw();
	}

    // Draw B-Spline Lines (using the same shader for Path Lines)
    for (vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it){
		// Draw model
		(*it)->Draw();
	}

	// Restore previous shader
	Renderer::SetShader((ShaderType) prevShader);

	Renderer::EndFrame();
}

//=================================================

Path* World::FindPath(ci_string pathName)
{
    for(std::vector<Path*>::iterator it = mPath.begin(); it < mPath.end(); ++it)
    {
        if((*it)->GetName() == pathName)
        {
            return *it;
        }
    }
    return nullptr;
}

BSpline* World::FindSpline(ci_string pathName)
{
    for(std::vector<BSpline*>::iterator it = mSpline.begin(); it < mSpline.end(); ++it)
    {
        if((*it)->GetName() == pathName)
        {
            return *it;
        }
    }
    return nullptr;
}

BSpline* World::FindSplineByIndex(unsigned int index)
{
    return mSpline.size() > 0 ? mSpline[index % mSpline.size()] : nullptr;
}

Model* World::FindModelByIndex(unsigned int index)
{
    return mModel.size() > 0 ? mModel[index % mModel.size()] : nullptr;
}