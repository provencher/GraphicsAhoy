////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//	4rce of Nature
//		Lighting and Rendering Engine
//
//	Contributors:
//		Eric Provencher
//		Rita Phom
//		Jordan Rutty
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "World.h"
#include "ParsingHelper.h"
#include "Renderer.h"


#include "StaticCamera.h"
#include "BSplineCamera.h"

#include "PlayerCamera.h"


//Models
#include "Models/GroupModel.h"
#include "Models/PlaneModel.h"
#include "Models/CubeModel.h"
#include "Models/Craters.h"
#include "Models/Terrain.h"
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


//World Constructor written by Eric
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
	float d = 0.4f;
	// setup lights
	/*
	Light spotlight;
	spotlight.position = glm::vec4(0, 10, 0, 1);
	spotlight.intensities = glm::vec3(0, 1, 2); //strong white light
	spotlight.attenuation = 0.1f;
	spotlight.ambientCoefficient = 0.0f; //no ambient light
	spotlight.coneAngle = 15.0f;
	spotlight.coneDirection = glm::vec3(0, -1, 0);
	gLights->push_back(spotlight);
	*/
	Light directionalLight;
	directionalLight.position = glm::vec4(15, 18, 0.6, 0); //w == 0 indications a directional light
	directionalLight.intensities = glm::vec3(d, d, d); 
	directionalLight.ambientCoefficient = 0.2f;
	gLights->push_back(directionalLight);

	Light light3;
	light3.position = glm::vec4(-15, 5, 15, 0); //w == 0 indications a directional light
	light3.intensities = glm::vec3(d/4.5, d/4, d/5); //weak yellowish light
	light3.ambientCoefficient = 0.06f;
	gLights->push_back(light3);


	glfwGetWindowSize(EventManager::GetWindow(), &width, &height);


	const float arr[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0 };

	biasMatrix = glm::make_mat4(arr);

	//Projection matrix used by light
	depthProjectionMatrix = glm::ortho<float>(-100, 100, -100, 100, -50, 50);

	//Projection matrix used by cameras
	projMat = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 150.0f);

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

	delete altCamera;

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

//Jordan + Rita Contribution
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
				//CubeModel* cube = new CubeModel();
				//cube->Load(iss);
				//mModel.push_back(cube);
			}
			else if( result == "sphere" ){
				// Load Sphere -----------
                SphereModel* sphere = new SphereModel();
                sphere->Load(iss);
                mModel.push_back(sphere);
			}
			else if( result == "path" ){
				// Load Path --------------
				Path* path = new Path();
				path->Load(iss);
                mPath.push_back(path);
			} 
			else if( result == "spline" ){
				// Load Spline -------------
				BSpline* path = new BSpline();
				path->Load(iss);
                mSpline.push_back(path);
			} 
			else if ( result.empty() == false && result[0] == '#'){
				// this is a comment line
			} 
			else {
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
	    }
	}
	input.close();
	

	


	//####################################################################################
	
	// Create World

	//------------------------------------------------------------------------------------
	GroupModel* ground = new GroupModel();
	//Basic World Generation Starter Kit By Jordan --------------------------------------
	if(1){ //Ground ===============================
		
		ground->SetName("Ground");


		//Generate Ground plate -----------------------------------------------------
		vec3 plateSize = vec3(vec3(200,1,200));
		Model* group = new GroupModel();
		group->SetPosition(vec3(0,-0.5f,0));
		ground->AddChild(group);

		//Generate Ground plate -----------------------------------------------------
		//vec3 plateSize = vec3(vec3(200,1,200));
		//Model* groundPlate = new CubeModel(vec3(0.6f));
		//groundPlate->SetScaling(plateSize);
		//group->AddChild(groundPlate);
		//Add objects to GroundPlate -------------------------------------------------
		//Drawing a terrain. To toggle this, enable "RenderTerrain()"  
		DrawTerrain(ground);
		
		//Generate 1 Ground plate, just have to spawn more and keep track of what to despawn
		for(int i=0; i< 40; i++){
			//Choose object to spawn
			Model* shape = new CubeModel(vec3(0.8f));
			

			int maxSize = 15;
			int minSize = 5;
			//Random shape
			vec3 randSize = vec3(
				rand() % (maxSize-minSize)+minSize,
				rand() % (maxSize-minSize)+minSize,
				rand() % (maxSize-minSize)+minSize
			);
			//Random position relative to plate
			vec3 randPos = vec3(
				(rand() % (int)plateSize.x) - 0.5f*plateSize.x, // center on plate
				randSize.y/2-0.5f,
				(rand() % (int)plateSize.z)- 0.5f*plateSize.z	// center on plate
			);

			//Spawn Shape
			shape->SetScaling(randSize);
			shape->SetPosition(randPos);
			shape->CreateDefaultCollisionCube();
			group->AddChild(shape);	
		}
		//AddChild("Ground", ground);
		mModel.push_back(ground);
	}	



	//for getting a better view of the Jet plane spawn one above map
	if(1){
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

	// Third Person Cube Character -------------------------
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

	// Create Camera -----------------------------------------
	PlayerCamera* newCam = new PlayerCamera(character);
	newCam->SetCameraRadius(7.0f);
	mCamera.push_back(newCam); //4
    //*note: to be moved into its own class
	////////////////////////////////////////////////////////

	/*
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
	*/

    LoadCameras();
	mCurrentCamera = 0;

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

	//Setup Alt Camera -- Eric contribution
	glm::vec3 pos = mCamera[1]->GetPosition();
	glm::vec3 look = mCamera[0]->GetLookAt();
	glm::vec3 up = mCamera[1]->GetUp();
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

//Eric and Rita Contribution
void World::Draw(){

	Renderer::BeginFrame(); //Default frame with black background
	//Renderer::BeginFrameFog(); // If RenderFog() is toggle, switch to this frame to have a gray sky (more realistic)

	//---------------------------------------------
	RenderShadows(); // Toggles the shadow
	RenderTerrain(); // Toggles the tiles texture
	//RenderFog(); // Toggles the fog feature 
	//RenderScene(); // Toggles the default scene
	//DrawPath();

	Renderer::EndFrame();
}

// Render fog to the scenery -- RITA contribution -- Currently unstable
void World::RenderFog(){

	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_FOG);
	glUseProgram(Renderer::GetShaderProgramID());

	RenderCommon();

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);
}

// Drawing a terrain -- Rita Contribution to add other objects in scene
void World::DrawTerrain(GroupModel *ground){
	//Create a simple terrain object
	Model* terrain = new Terrain();
	terrain->SetScaling(vec3(200,0.01,200));
	terrain->SetPosition(vec3(0, 0, 0));
	terrain->SetRotation(vec3(0, 0, 1), 360.0f);
	ground->AddChild(terrain);

	//Creating craters for decor (needs to be adjust)
	Model* crater = new Craters();
	crater->SetScaling(vec3(2, 2, 2));
	crater->SetPosition(vec3(5, 1, 5));		//Rita I fixed this - Jordan, cubes where floating
	crater->SetRotation(vec3(0, 1, 0), 50.0f);
	ground->AddChild(crater);

	Model* crater1 = new Craters();
	crater1->SetScaling(vec3(2, 2, 2));
	crater1->SetPosition(vec3(5, 3, 5));
	crater->SetRotation(vec3(0, 1, 0), 35.0f);
	ground->AddChild(crater1);

	Model* crater2 = new Craters();
	crater2->SetScaling(vec3(2, 2, 2));
	crater2->SetPosition(vec3(5, 1, 8));
	crater->SetRotation(vec3(0, 1, 0), 35.0f);
	ground->AddChild(crater2);
}


//Common Rendering for all aspects of the program
//Written by Eric
void World::RenderCommon(){
	//Material Attributes uniform
	GLuint MaterialID = glGetUniformLocation(Renderer::GetShaderProgramID(), "materialCoefficients");

	//WorldCamPosition
	GLuint CamPos = glGetUniformLocation(Renderer::GetShaderProgramID(), "worldCamPos");

	//Pull CurrentLookAt vector from camera;	
	camPos = mCamera[mCurrentCamera]->GetPosition();
	glUniform3fv(CamPos, 1, &camPos[0]);

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	mat4 VP = projMat*GetCamera()->GetViewMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	//WorldCamView
	GLuint CamView = glGetUniformLocation(Renderer::GetShaderProgramID(), "CameraView");
	mat4 camView = GetCamera()->GetViewMatrix();
	glUniformMatrix4fv(CamView, 1, GL_FALSE, &camView[0][0]);

	//Light Projection
	GLuint LightProj = glGetUniformLocation(Renderer::GetShaderProgramID(), "LightVP");
	mat4 lightProject = biasMatrix * depthProjectionMatrix * altCamera->GetViewMatrix();
	glUniformMatrix4fv(LightProj, 1, GL_FALSE, &lightProject[0][0]);

	//Lights
	GLuint NumLights = glGetUniformLocation(Renderer::GetShaderProgramID(), "numLights");
	int numberOfLights = (int)gLights->size();
	glUniform1i(NumLights, numberOfLights);

	vector<GLuint> gluints;
	string uniformName;
	const char* c_str;

	glm::vec3 temp;
	glm::vec4 v4f;

	//Multiple Light loading 
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

	// Draw models
	vec4 matC;
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it){

		matC = (*it)->materialConst;
		glUniform4fv(MaterialID, 1, &matC[0]);
		// Draw model
		(*it)->Draw();
	}
}


// Render the terrain to the scenery
void World::RenderTerrain(){

	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_SOLID_COLOR);
	glUseProgram(Renderer::GetShaderProgramID());

	//RenderShadows();
	RenderCommon();
	

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);

}

//Default scenery
void World::RenderScene(){
	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_SOLID_COLOR);
	glUseProgram(Renderer::GetShaderProgramID());

	RenderCommon();

	// Restore previous shader
	Renderer::SetShader((ShaderType)prevShader);
}

//Eric Contribution -- Shadows
//Renders scene from the perspective of sun, stores in framebuffer1 
//Stores framebuffer1 in texture
//Passes texture as sampler uniform to regular shaders
void World::RenderShadows()
{

	//for (size_t i = 0; i < gLights->size(); ++i){
	glBindFramebuffer(GL_FRAMEBUFFER, 1);
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	altCamera->SetPosition((glm::vec3)(*gLights)[0].position);
	//Dynamic light movement
	//altCamera->SetLookAt(GetCamera()->GetLookAt());
	//altCamera->SetUp(GetCamera()->GetUp());

	//static light
	//altCamera->SetLookAt(mCamera[1]->GetLookAt());
	//altCamera->SetUp(mCamera[1]->GetUp());

	//GetCamera()->SetPosition(altCamera->GetPosition());

	glCullFace(GL_FRONT);
	DrawShadow();
	glCullFace(GL_BACK);

	//}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, width, height);
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Renderer::SetShader(SHADER_SOLID_COLOR);
	//glUseProgram(Renderer::GetShaderProgramID());
	//std::cout << "ShaderID: " << (int)Renderer::GetShaderProgramID() << endl;
	GLuint shadowMapHandle = glGetUniformLocation(Renderer::GetShaderProgramID(), "R_shadowMap");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 1);
	glUniform1i(shadowMapHandle, 0);
}

//Shadow shader uniforms and rendering
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

void World::DrawPath()
{
	// Draw Path Lines

	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_PATH_LINES);
	glUseProgram(Renderer::GetShaderProgramID());

	// Send the view projection constants to the shader
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	mat4 ViewPorj = projMat * GetCamera()->GetViewMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &ViewPorj[0][0]);

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

