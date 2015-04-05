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

//Models
#include "Models/GroupModel.h"
#include "Models/PlaneModel.h"
#include "Models/CubeModel.h"
#include "Models/SphereModel.h"
#include "Models/Billboard.h"
#include "Models/RazorbackModel.h";
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
	// Material Coefficients
	ka = 0.39f;
	kd = 0.46f;
	ks = 0.82f;
	n = 75.0f;

	//Create light Vector
	gLights = new vector<Light>();

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

	gLights->push_back(spotlight);
	gLights->push_back(directionalLight);
	gLights->push_back(light3);



	glfwGetWindowSize(EventManager::GetWindow(), &width, &height);

	const float arr[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0 };

	biasMatrix = glm::make_mat4(arr);

	// Compute the MVP matrix from the light's point of view
	depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	

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


///=====================================================
string LightNameBuilder(string name, int index){

	std::ostringstream ss;
	ss << "allLights[" << index << "]." << name;
	return ss.str();
}

//=================================================
void World::LoadScene(const char * scene_path){
	
	

	
	
	//Load Objects from File =====================
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





	//####################################################################################
	
	// Create World

	////////////////////////////////////////////////////
	Model* m = new RazorbackModel();
	m->SetPosition(vec3(0.0f, 1, -80.0f));
	mModel.push_back(m);


	GroupModel* world = new GroupModel();

	if(1){ //Ground ===============================
		GroupModel* ground = new GroupModel();
		vec3 plateSize = vec3(vec3(200,1,200));


		//world renders in a 3x3 square of plates
		Model* groundPlates[3][3];
		
		//counters
		int i = 0, j = 0;

		for (i = 0; i < 3;) {
			//xPos and zPos describe the coordinates of the plate on the 3x3 grid; X: 0 = left, 1 = mid, 2 = right; Z: 0 = front, 1 = mid, 2 = rear
			float xPos = 1;
			float zPos = 1;
			for (j = 0; j < 3;) {
				groundPlates[i][j] = new CubeModel(vec3(0.6f));
				groundPlates[i][j]->SetScaling(plateSize);
				//front
				if (j == 0) {
					zPos = plateSize.z / 2;
				}
				else if (j == 2) {
					zPos = - plateSize.z / 2;
				}
				//left side
				if (i == 0) {
					xPos = plateSize.x / 2;
				}
				//right side
				else if (i == 2) {
					xPos = -plateSize.x / 2;
				}

				groundPlates[i][j]->SetPosition(vec3(xPos, -0.5f, zPos));

				ground->AddChild(groundPlates[i][j]);

				j++;
			}
	
			i++;
		}

		//m->SetRotation(vec3(0,0,1), 90.0f);
		
		//continuous tracking of the ground plate at the center of the square
		setGroundModel(groundPlates[1][1]);
		//Billboard
		//============================================
		for(int i=0; i< 50; i++){
			
			vec3 randSize = vec3(
				rand() % 10+3,
				rand() % 10+3,
				rand() % 10+3
			);
			vec3 randPos = vec3(
				(rand() % (int)plateSize.x) - 0.5f*plateSize.x,
				randSize.y/2,
				(rand() % (int)plateSize.z) - 0.5f*plateSize.z
			);

			

			Model* shape = new CubeModel(vec3(0.8f));
			int x = 1.0f;
			shape->SetPosition(randPos);
			shape->SetScaling(randSize);
			shape->CreateDefaultCollisionCube();
			ground->AddChild(shape);

			ground->AddChild("dog",shape);
			
		}

		world->AddChild("Ground", ground);
	}	
	
	//(rand() % 10 + 50)/10;

	mModel.push_back(world);



	//static model of the player's plane, debug purposes
	if(0){
		//Big Plane
		GroupModel* character = new PlaneModel();
		//character->SetRotation(vec3(1,0,0), 90);//change thirs person to accomidate 
		float scale = 5;
		character->SetScaling(vec3(scale, scale, scale));
		character->SetPosition(vec3(20, 20, 0));
		character->SetRotation(vec3(0, 1, 0),  0);
		character->SetSpeed(14.0f);	//Should move to camera
		mModel.push_back(character);
	}













	

	////////////////////////////////////////////////////////

	// PLAYER

	// Third Person Plane Character -------------------------
    GroupModel* character = new PlaneModel();
	//character->SetRotation(vec3(1,0,0), 90);//change thirs person to accomidate 
	float scale = 0.5f;
	character->SetScaling(vec3(scale, scale, scale));
	character->SetPosition(vec3(0.0f, 1, -90.0f));
	character->SetRotation(vec3(0, 1, 0),  90);
	character->CreateDefaultCollisionCube();
	character->ReScaleCollisionCube(vec3(4));
	character->SetSpeed(25.0f);	//Should move to camera
    mModel.push_back(character);

	this->playerModel = character;


	
	// Create Camera -----------------------------------------
	ThirdPersonCamera* newCam = new ThirdPersonCamera(character);
	newCam->SetCameraRadius(7.0f);
	mCamera.push_back(newCam); //4
    //*note: to be moved into its own class
	////////////////////////////////////////////////////////

	


	//Billboard
	//============================================
	for(int i=0; i< 5; i++){
		float randf = rand() % 10+5;
		if(1){
			BillBoard* myBillBoard = new BillBoard();
			myBillBoard->SetPosition(vec3(randf,15+randf,randf));
			int x = 1.0f;
			myBillBoard->SetScaling(vec3(x,x,x));
			myBillBoard->SetLookAtCamera(newCam);
			//SetLookat
			//SetEye
			mModel.push_back(myBillBoard);
		}
	}










    
    LoadCameras();
	mCurrentCamera = 0;
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

	//Setup Alt Camera
	glm::vec3 pos = mCamera[0]->GetPosition();
	glm::vec3 look = mCamera[0]->GetLookAt();
	glm::vec3 up = mCamera[0]->GetUp();
	altCamera = new StaticCamera(pos, look, up);


    // BSpline Camera --------------------------------------
    BSpline* spline = FindSpline("\"RollerCoaster\"");
    if (spline == nullptr)
        spline = FindSplineByIndex(0);
 
    if (spline != nullptr)
        mCamera.push_back(new BSplineCamera(spline , 5.0f)); //5
    
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

	if (getPlayerModel()->GetPosition().z > getGroundModel()->GetPosition().z - 50) { //offset check by 50 to keep render out of view
		//TODO TAGS: GROUNDMODEL GENERATE CHARACTER
		generateWorldSection(getPlayerModel(), getGroundModel()); 

		printf("World section generated.");

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

void World::DrawShadow(){
	//Set Shaders to shadows
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_SHADOW);
	glUseProgram(Renderer::GetShaderProgramID());



	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint ViewLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "LightVM");
	mat4 lightView = altCamera->GetViewMatrix();
	glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, &lightView[0][0]);

	//ProjMat
	GLuint Projection = glGetUniformLocation(Renderer::GetShaderProgramID(), "Projection");
	glUniformMatrix4fv(Projection, 1, GL_FALSE, &depthProjectionMatrix[0][0]);





	//Draw All models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it){
		(*it)->Draw();
	}

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);

}

void World::Draw()
{
	Renderer::BeginFrame();
	
	
	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	//Material Attributes uniform
	GLuint MaterialID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialCoefficients");

	//WorldCamPosition
	GLuint CamPos = glGetUniformLocation(Renderer::GetShaderProgramID(), "worldCamPos");
	glUniform3fv(CamPos, 1, &camPos[0]);

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
	
	//Look for WorldTransform in the Vertex Shader
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");	
	mat4 WorldMatrix = mModel[0]->GetWorldMatrix();
	//glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &WorldMatrix[0][0]);

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");	
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);





	// Draw models
	vec4 matC;
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it){

		matC = (*it)->materialConst;
		glUniform4fv(MaterialID, 1, &matC[0]);
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



int World::AddLight(vec4 pos, vec3 color){
	//*
	// setup lights
	Light l;


	l.position = pos;
	l.intensities = color; 
	/*
	l.attenuation = 0.5f;
	l.ambientCoefficient = 0.0f; //no ambient light
	l.coneAngle = 5.0f;
	l.coneDirection = glm::vec3(0, -1, 0);
	//*/


	gLights->push_back(l);
	return gLights->size()-1;
}

void World::UpdateLight(int index, glm::vec4 pos, glm::vec3 color){
	(*gLights)[index].position = pos;
	(*gLights)[index].intensities = color;
	(*gLights)[index].attenuation = 0.1f;
	(*gLights)[index].ambientCoefficient = 0.0f; //no ambient light
}
	
	
void World::UpdateLight(int index, glm::vec4 pos, glm::vec3 color, float attenuation, float ambientCoefficient){
	(*gLights)[index].position = pos;
	(*gLights)[index].intensities = color;
	(*gLights)[index].attenuation = attenuation;
	(*gLights)[index].ambientCoefficient = ambientCoefficient;
}

void World::UpdateLight(int index, glm::vec4 pos, glm::vec3 color, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection){
	(*gLights)[index].position = pos;
	(*gLights)[index].intensities = color;
	(*gLights)[index].attenuation = attenuation;
	(*gLights)[index].ambientCoefficient = ambientCoefficient; 
	(*gLights)[index].coneAngle = coneAngle;
	(*gLights)[index].coneDirection = coneDirection;
}

void World::RemoveLight(int index){
	gLights->erase(gLights->begin()+index); // NOTE gLights should be map, issue will arise when deleting corrupting indexes that follow
}

Model* World::getPlayerModel() {
	return this->playerModel;
}

void World::generateWorldSection(Model* character, Model* currentGroundPlate) {
	//Todo finish generation in function
	vec3 startPos = character->GetPosition(); //The starting position is determined by the current location of the player
	vec3 groundPos = getGroundModel()->GetPosition(); //The ground position is centered on the ground model
	vec3 groundScaling = vec3(200, 1, 200); //Scaling used to determine the length from one end of the model to the other e.g. distance at which to render the new model

	Model* groundBase = new GroupModel();

	//Maintain a 3x3 grid of plates around the player
	Model* plateArray[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			plateArray[i][j] = new CubeModel(vec3(0.6f));
		}
	}

	//[1][1] is the midpoint of the cube described by the 3x3 array
	plateArray[1][1] = currentGroundPlate;


	
	//Set all positions dependent on scaling of previous positions
	groundBase->SetPosition(groundPos + vec3(0, 0, groundScaling.z / 2));

	setGroundModel(groundBase);

	//add ground and its children to render queue
	mModel.push_back(groundBase);

}

void World::setGroundModel(Model* model) {
	groundModel = model;
}

Model* World::getGroundModel() {
	return groundModel;
}
