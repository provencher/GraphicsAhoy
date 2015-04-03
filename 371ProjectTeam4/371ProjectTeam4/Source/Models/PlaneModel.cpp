//
// COMP 371 Assignment Framework
//
// Created by Jordan Rutty
//
//
//

#include "PlaneModel.h"
#include "PropellerModel.h"
#include "CubeModel.h"
#include "SphereModel.h"
#include "TorrentModel.h"
#include "LightModel.h";
#include "../Renderer.h"
#include "particleEmitter.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

PlaneModel::PlaneModel(vec3 size) : GroupModel(){

	//note if(1){} used to seperate scope to allow rapid development




	
	vec3 color =  vec3(1,0,0);
	if(1){//body
		SphereModel* model = new SphereModel(color);
		model->SetScaling(vec3(0.8f, 0.5f, 2.5f));
		model->SetPosition(vec3(0.0f, 0.0f, 0.0f));
		//model->SetRotation(vec3(0.0f, 1.0f, 0.0f), 45.0f);
		AddChild("body", model);
	}


	if(1){//cockpit
		GroupModel* group = new GroupModel();
		if(1){ //glass
			SphereModel* model = new SphereModel(vec3(0.4f,0.4f,0.4f));
			model->SetScaling(vec3(0.5f, 0.4f, 1.7f));
			model->SetPosition(vec3(0.0f, 0.3f, 0.0f));
			//model->SetRotation(vec3(0.0f, 1.0f, 0.0f), 45.0f);
			group->AddChild("glass", model);
		}
		if(1){ //back
			SphereModel* model = new SphereModel(color);
			model->SetScaling(vec3(0.5f, 0.4f, 2.0f));
			model->SetPosition(vec3(0.0f, 0.3f, -0.4f));
			//model->SetRotation(vec3(0.0f, 1.0f, 0.0f), 45.0f);
			group->AddChild("back",model);
		}
		AddChild("cockpit", group);
		group->mRotationAngleX = -5.0f;
	}


	//wing
	SphereModel* wing = new SphereModel();
	wing->SetScaling(vec3(4.0f, 0.1f, 0.5f));
	wing->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	//sphere->SetRotation(vec3(0.0f, 1.0f, 0.0f), 45.0f);
	AddChild("wing", wing);




	//Torrent Model ----------------------------------------
	GroupModel* torrentLayer = new GroupModel();
	if(1){
		vec3 torrentSize = vec3(0.5,0.5,1.4f);
		vec3 boxsize = vec3(0.3f, 0.3f, 0.5f);
		//left gun
		if(1){
			GroupModel* g = new GroupModel();
			if(1){
				GroupModel* m = new TorrentModel();
				m->SetPosition(vec3(0,0,0.5f));
				m->SetRotation(vec3(0.0f,0.0f,1.0f), 0.0f);
				m->SetScaling(torrentSize);		
				m->SetRotationSpeed(2*360.0f);
				g->AddChild(m);
			}
			if(1){
				Model* m = new CubeModel(vec3(0.4f));
				m->SetPosition(vec3(0,0,0.0f));
				m->SetScaling(boxsize);
				g->AddChild(m);
			}
			//g->SetScaling(vec3(2));
			g->SetPosition(vec3(-1.7f,0.2f,0.0f));
			torrentLayer->AddChild(g);

		}

		//right gun
		if(1){
			GroupModel* g = new GroupModel();
			if(1){
				GroupModel* m = new TorrentModel();
				m->SetPosition(vec3(0,0,0.5f));
				m->SetRotation(vec3(0.0f,0.0f,1.0f), 0.0f);
				m->SetScaling(torrentSize);		
				m->SetRotationSpeed(2*360.0f);
				g->AddChild(m);
			}
			if(1){
				Model* m = new CubeModel(vec3(0.4f));
				m->SetPosition(vec3(0,0,0.0f));
				m->SetScaling(boxsize);
				g->AddChild(m);
			}
			//g->SetScaling(vec3(2));
			g->SetPosition(vec3(1.7f,0.2f,0.0f));
			torrentLayer->AddChild(g);
		}
	
		torrentLayer->SetPosition(vec3(0,-0.2f,0.2));
		AddChild("engines", torrentLayer);
	}


	



	//*
	if(1){
		vec3 thrusterColor = vec3(0.4,0.4,0.4);
		GroupModel* group = new GroupModel();
		
		if(1){//thruster1
			CubeModel* model= new CubeModel(thrusterColor);
			model->SetScaling(vec3(0.7f, 0.3f, 2));
			model->SetPosition(vec3(-0.4f,	-0.1f,	-1.4f));
			model->SetRotation(vec3(0.0f,	1.0f,	0.0f), 5.0f);
			group->AddChild("left", model);
		}
		if(1){//thruster2
			CubeModel* model= new CubeModel(thrusterColor);
			model->SetScaling(vec3(0.7f, 0.3f, 2));
			model->SetPosition(vec3(0.4f,	-0.1f,	-1.4f));
			model->SetRotation(vec3(0.0f,	1.0f,	0.0f), -5.0f);
			group->AddChild("right", model);
		}
		AddChild("thruster", group);
		
		//Left afterburner
		if(1){
			ParticleEmitter* pe = new ParticleEmitter();
			pe->SetPosition(vec3(-0.5f,-0.1f,-2.4));
			AddChild("left-afterburner", pe);
		}

		//Right afterburner
		if(1){
			ParticleEmitter* pe = new ParticleEmitter();
			pe->SetPosition(vec3(0.5f,-0.1f,-2.4));
			AddChild("right-afterburner", pe);
		}
	}
	//*/




	
	//--------------------------------------------------
	//* Front Propeller
	if(true){
		GroupModel* propeller = new GroupModel();
		float bladeLength = 0.6f;
		int blades = 8;
		for(int i=0; i<blades;i++){	
			GroupModel* blade = new GroupModel();
				SphereModel* sphere = new SphereModel();
				sphere->SetScaling(vec3(bladeLength, 0.06f, 0.1f));
				sphere->SetPosition(vec3(bladeLength/2, 0.0f, 0.0f));
				sphere->SetRotation(vec3(1.0f, 0.0f, 0.0f), 45.0f);
			blade->AddChild(sphere);
			blade->SetRotation(vec3(0.0f,0.0f,2.0f), i*(360.0f/blades));
			propeller->AddChild(blade);
		}
	
		propeller->SetPosition(vec3(0.0f,0.0f,2.5f));
		propeller->SetRotation(vec3(0.0f,0.0f,1.0f), 0.0f);
		propeller->SetRotationSpeed(5.0f*180.0f);
		AddChild("propeller", propeller);
	}
	//*/


	//Rudder
	GroupModel* rudder = new GroupModel();
	vec3 rudderSize = vec3(0.07f,1.15f, 1.0f);
	vec3 rudderColor = vec3(1,0,0);
	if(1){//Right
		GroupModel* group = new GroupModel();
			CubeModel* model = new CubeModel(rudderColor);
			model->SetPosition(vec3(0.0f,0.5f,-2.0f));
			model->SetRotation(vec3(1.0f,0.0f,0.0f), -30.0f);	
			model->SetScaling(rudderSize);
		group->AddChild(model);
		group->SetRotation(vec3(0,0,1), 45.0f);
		rudder->AddChild(group);
	}
	if(1){//Left
		GroupModel* group = new GroupModel();
			CubeModel* model = new CubeModel(rudderColor);
			model->SetPosition(vec3(0.0f,0.5f,-2.0f));
			model->SetRotation(vec3(1.0f,0.0f,0.0f), -30.0f);	
			model->SetScaling(rudderSize);
		group->AddChild(model);
		group->SetRotation(vec3(0,0,1), -45.0f);
		rudder->AddChild(group);
	}
	if(0){//Center
		GroupModel* group = new GroupModel();
			CubeModel* model = new CubeModel();
			model->SetPosition(vec3(0.0f,-0.5f,-2.0f));
			model->SetRotation(vec3(1.0f,0.0f,0.0f), -30.0f);	
			model->SetScaling(rudderSize);
		group->AddChild(model);
		//rudderBL->SetRotation(vec3(0,0,1), -45.0f);
		rudder->AddChild(group);
	}
	rudder->SetRotation(vec3(0.0f,0.0f,1.0f), 0.0f);
	AddChild("rudder",rudder);
	//*////////////////////////////////////////////////////
	//$#%^$#$^Y$T#%^&%$#W$^&$#%^&%$#%^&&%^$#$%^&%%$#$$^%#$%
	//Model Testing :: Transforms on multiple layers
	


	




	//Light ---------------------------------------------
	vec4 tempPos = vec4(0,0,0,1);
	vec3 lame = vec3(tempPos.x, tempPos.y, tempPos.z);
	Model* lightModel = new LightModel(lame, 2.0f*vec3(1,0.6f,0.4f));
	AddChild(lightModel);
	lightModel->SetPosition(vec3(0,0.2f,-2.5f));



	//--------------------------------------------------
	/* 2 rotating offset spheres
	if(1){
		GroupModel* propeller = new GroupModel();
		float bladeLength = 0.6f;
		int blades = 2;
		for(int i=0; i<blades;i++){	
			//Build blade -------------------------------
			GroupModel* blade = new GroupModel();
				SphereModel* sphere2 = new SphereModel();
				sphere2->SetScaling(vec3(	0.2,	0.5f,	1.0f));
				sphere2->SetPosition(vec3(	1.5f,	0.0f,	0.0f));
				sphere2->SetRotation(vec3(	1.0f,	2.0f,	0.0f), 0.0f);
				blade->AddChild(sphere2);
			blade->SetRotation(vec3(0.0f,0.0f,2.0f), i*(360.0f/blades));
			propeller->AddChild(blade);
		}
		propeller->SetPosition(vec3(0.0f,0.0f,-2.5f));
		propeller->SetRotation(vec3(0.0f,0.0f,1.0f), 0.0f);
		propeller->SetRotationSpeed(0.5f*360.0f);
		AddChild(propeller);
	}
	//*/



	//*/
}

PlaneModel::~PlaneModel(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void PlaneModel::Update(float dt){
	GroupModel::Update(dt);

	
}

void PlaneModel::Draw(){
	GroupModel::Draw();
}

bool PlaneModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty()){
		return true;
	} else {
		return Model::ParseLine(token);
	}
}
