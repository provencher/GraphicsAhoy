//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "World.h"
#include "ParsingHelper.h"
#include "Renderer.h"


#include "StaticCamera.h"
#include "BSplineCamera.h"
#include "ThirdPersonCamera.h"

#include "Models/GroupModel.h"
#include "Models/PlaneModel.h"
#include "Models/CubeModel.h"
#include "Models/CubeModelGround.h"
#include "Models/SphereModel.h"
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
	SetTexture("shadowMap", new Texture(1024, 768, 0, GL_TEXTURE_2D, GL_NEAREST, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, true, GL_DEPTH_ATTACHMENT));

	// Material Coefficients
	ka = 0.39f;
	kd = 0.46f;
	ks = 0.82f;
	n = 75.0f;

	//Create light Vector
	gLights = new vector<Light>();

	// setup lights
	Light spotlight;
	spotlight.position = glm::vec4(0, 10, 0, 1);
	spotlight.intensities = glm::vec3(2, 2, 3); //strong white light
	spotlight.attenuation = 0.1f;
	spotlight.ambientCoefficient = 0.1f; //no ambient light
	spotlight.coneAngle = 5.0f;
	spotlight.coneDirection = glm::vec3(0, -1, 0);

	Light directionalLight;
	directionalLight.position = glm::vec4(5, 10, 0.6, 0); //w == 0 indications a directional light
	directionalLight.intensities = glm::vec3(0.5, 0.5, 0.5); //weak yellowish light
	directionalLight.ambientCoefficient = 0.06f;	

	
	//directionalLight.m_shadowInfo = glm::ortho(-40.0f, -40.f, -40.f, -40.f, -40.0f, 1.0f);

	Light light3;
	light3.position = glm::vec4(-5, 5, -0.6, 0); //w == 0 indications a directional light
	light3.intensities = glm::vec3(0.5, 0.5, 0.5); //weak yellowish light
	light3.ambientCoefficient = 0.06f;

	gLights->push_back(spotlight);
	gLights->push_back(directionalLight);
	gLights->push_back(light3);


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
			if (result == "cube"){
				// Load Box --------------
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				mModel.push_back(cube);
			}else if (result == "cube_ground"){
				// Load Cube Box --------------
				CubeModelGround* ground = new CubeModelGround();
				ground->Load(iss);
				mModel.push_back(ground);
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
    
	// Create Character -----------------------------------
	////////////////////////////////////////////////////////

	// Third Person Cube Character -------------------------
    GroupModel* character = new PlaneModel();
	//character->SetRotation(vec3(1,0,0), 90);//change thirs person to accomidate 
	float scale = 0.5f;
	character->SetScaling(vec3(scale, scale, scale));
	character->SetPosition(vec3(10.0f, 0.5f, 0.0f));
	character->SetSpeed(7.0f);	//Should move to camera
    mModel.push_back(character);


	
	// Create Camera -----------------------------------------
	ThirdPersonCamera* newCam = new ThirdPersonCamera(character);
	newCam->SetCameraRadius(7.0f);
	mCamera.push_back(newCam); //4
    //*note: to be moved into its own class
	////////////////////////////////////////////////////////	


    // BSpline Camera --------------------------------------
    BSpline* spline = FindSpline("\"RollerCoaster\"");
    if (spline == nullptr)
        spline = FindSplineByIndex(0);
 
    if (spline != nullptr)
        mCamera.push_back(new BSplineCamera(spline , 5.0f)); //5
    
    
    mCurrentCamera = 0;


	//Setup Alt Camera
	glm::vec3 pos = mCamera[0]->getCamPos();
	glm::vec3 look = mCamera[0]->getLookAt();
	glm::vec3 up = mCamera[0]->getCamUpV();
	altCamera = new StaticCamera(pos, look, up);
}
Camera* World::GetCamera(){
	//? may require checking if nullptr
	return mCamera[mCurrentCamera];
}

//=================================================
void World::Update(float dt)
{
	// User Inputs
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS){
		mCurrentCamera = 0;
	} else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS){
		if (mCamera.size() > 1){
			mCurrentCamera = 1;
		}
	} else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS){
		if (mCamera.size() > 2){
			mCurrentCamera = 2;
		}
	} else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_4 ) == GLFW_PRESS){
        // Spline camera
		if (mCamera.size() > 3 && mSpline.size() > 0){
			mCurrentCamera = 3;
		}
	} else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_5 ) == GLFW_PRESS){
        // Spline camera
		if (mCamera.size() > 4 && mModel.size() > 0){
			mCurrentCamera = 4;
		}
	}


	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	//Pull CurrentLookAt vector from camera;	
	camPos = mCamera[mCurrentCamera]->getCamPos();

	//std::cout << "x " << camPos.x << "y " << camPos.y << "z " << camPos.z << endl;

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it){
		(*it)->Update(dt);
	}
}


void World::Draw()
{
	Renderer::BeginFrame();

	unsigned int prevShader = Renderer::GetCurrentShader();
	


	Renderer::SetShader(SHADER_LIGHT);
	glUseProgram(Renderer::GetShaderProgramID());

	

	DrawObjects();

	
	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);

	for (size_t i = 0; i < gLights->size(); ++i){

		GetTexture("shadowMap")->BindAsRenderTarget();
		glClear(GL_DEPTH_BUFFER_BIT);
	
		altCamera->setCamPos((glm::vec3)(*gLights)[i].position);	


		//Set Camera to altCam
		//Camera* tempCam = mCamera[mCurrentCamera];
		//mCamera[mCurrentCamera] = altCamera;	

		//DrawShadow();	

		//Restore camera
		//mCamera[mCurrentCamera] = tempCam;
	}

	Renderer::BindAsRenderTarget();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	DrawObjects();

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glDisable(GL_BLEND);

	DrawPathLines();
	
	Renderer::EndFrame();
}

void World::DrawShadow(){
	//Set Shaders to shadows
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_SHADOW);
	glUseProgram(Renderer::GetShaderProgramID());	

	//Look for WorldTransform in the Vertex Shader
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	mat4 WorldMatrix = mModel[0]->GetWorldMatrix();
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	lightVP = altCamera->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &lightVP[0][0]);


	GLuint shadowMapHandle = glGetUniformLocation(Renderer::GetShaderProgramID(), "R_shadowMap");
	//glActiveTexture(GL_TEXTURE0);
	glUniform1i(shadowMapHandle, 0);
	

	Texture* tex = GetTexture("shadowMap");
	//glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);

	//Draw All models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it){	
	
		(*it)->Draw();
	}

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);
	
}

string LightNameBuilder(string name, int index){

	std::ostringstream ss;
	ss << "allLights[" << index << "]." << name;
	return ss.str();
}

void World::DrawObjects(){
	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	//Material Attributes uniform
	GLuint MaterialID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialCoefficients");
	glUniform4f(MaterialID, ka, kd, ks, n);

	//WorldCamPosition
	GLuint CamPos = glGetUniformLocation(Renderer::GetShaderProgramID(), "worldCamPos");
	glUniform3fv(CamPos, 1, &camPos[0]);

	//Look for WorldTransform in the Vertex Shader
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	mat4 WorldMatrix = mModel[0]->GetWorldMatrix();
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);
	
	//Light View Proction	
	GLuint LightVP = glGetUniformLocation(Renderer::GetShaderProgramID(), "LightVP");
	mat4 altVP = altCamera->GetViewProjectionMatrix();
	glUniformMatrix4fv(LightVP, 1, GL_FALSE, &altVP[0][0]);


	//Lights
	GLuint NumLights = glGetUniformLocation(Renderer::GetShaderProgramID(), "numLights");
	int numberOfLights = (int)gLights->size();
	glUniform1i(NumLights, numberOfLights);

	vector<GLuint> gluints;
	string uniformName;
	const char* c_str;

	glm::vec3 temp;
	glm::vec4 v4f;


	//Stores the light parameters of each light
	for (size_t i = 0; i < numberOfLights; ++i){
		//Initialize vector to hold all 6 parameters
		gluints = vector<GLuint>(6);

		v4f = (*gLights)[i].position;
		uniformName = LightNameBuilder("position", i);
		gluints[0] = glGetUniformLocation(Renderer::GetShaderProgramID(), (c_str = uniformName.c_str()));
		glUniform4fv(gluints[0], 1, &v4f[0]);

		uniformName = LightNameBuilder("intensities", i);
		temp = (*gLights)[i].intensities;
		gluints[1] = glGetUniformLocation(Renderer::GetShaderProgramID(), (c_str = uniformName.c_str()));
		glUniform3fv(gluints[1], 1, &temp[0]);

		uniformName = LightNameBuilder("attenuation", i);
		gluints[2] = glGetUniformLocation(Renderer::GetShaderProgramID(), (c_str = uniformName.c_str()));
		glUniform1f(gluints[2], (*gLights)[i].attenuation);

		uniformName = LightNameBuilder("ambientCoefficient", i);
		gluints[3] = glGetUniformLocation(Renderer::GetShaderProgramID(), (c_str = uniformName.c_str()));
		glUniform1f(gluints[3], (*gLights)[i].ambientCoefficient);

		uniformName = LightNameBuilder("coneAngle", i);
		gluints[4] = glGetUniformLocation(Renderer::GetShaderProgramID(), (c_str = uniformName.c_str()));
		glUniform1f(gluints[4], (*gLights)[i].coneAngle);

		temp = (*gLights)[i].coneDirection;
		uniformName = LightNameBuilder("coneDirection", i);
		gluints[5] = glGetUniformLocation(Renderer::GetShaderProgramID(), (c_str = uniformName.c_str()));
		glUniform3fv(gluints[5], 1, &temp[0]);
	}


	// Draw models With all lights established
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it){
		// Draw model
		(*it)->Draw();
	}

	
	
}

void World::DrawPathLines(){
	// Draw Path Lines

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	mat4 VP = GetCamera()->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

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
	Renderer::SetShader((ShaderType)prevShader);
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