//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "PlaneModel.h"
#include "PropellerModel.h"
#include "CubeModel.h"
#include "SphereModel.h"
#include "TorrentModel.h"
#include "../Renderer.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

PlaneModel::PlaneModel(vec3 size) : GroupModel(){
	//Build heigharchical cube model
	//note if(1){} used to seperate sccope to allow rapid development


	/* //Wings Version 1
	GroupModel* wings = new GroupModel();
	vec3 wingscale = glm::vec3(5.3f,	0.15f,	1.0f);
	float hpos = 1.3;
	float rot = 45.0f;

	CubeModel* leftwing= new CubeModel();
	leftwing->SetScaling(wingscale);
	leftwing->SetPosition(glm::vec3(hpos,	0.0f,	0.0f));
	leftwing->SetRotation(glm::vec3(0.0f,	1.0f,	0.0f), rot);
	wings->AddChild(leftwing);
	
	CubeModel* rightwing= new CubeModel();
	rightwing->SetScaling(wingscale);
	rightwing->SetPosition(glm::vec3(-hpos,	0.0f,	0.0f));
	rightwing->SetRotation(glm::vec3(0.0f,	1.0f,	0.0f), -rot);
	wings->AddChild(rightwing);

	wings->SetPosition(glm::vec3(0.0f,0.0f,0.0f));
	AddChild(wings);
	//*/
	//Torrent Model ----------------------------------------
	GroupModel* torrentLayer = new GroupModel();
	float siz = 1.4;
	glm::vec3 torrentSize = glm::vec3(siz,siz,siz);
	//
	GroupModel* torrent1 = new TorrentModel();
	torrent1->SetPosition(glm::vec3(-1.7f,-0.2f,0.0f));
	torrent1->SetRotation(glm::vec3(0.0f,0.0f,1.0f), 0.0f);
	torrent1->SetScaling(torrentSize);		
	torrent1->SetRotationSpeed(2*360.0f);
	
	GroupModel* torrent2 = new TorrentModel();
	torrent2->SetPosition(glm::vec3(1.7f,-0.2f,0.0f));
	torrent2->SetRotation(glm::vec3(0.0f,0.0f,1.0f), 0.0f);
	torrent2->SetRotationSpeed(-2*360.0f);
	torrent2->SetScaling(torrentSize);

	torrentLayer->AddChild(torrent2);
	torrentLayer->AddChild(torrent1);
	torrentLayer->SetPosition(vec3(0,-0.2f,0));
	AddChild(torrentLayer);



	

	//Body
	SphereModel* body = new SphereModel();
	body->SetScaling(glm::vec3(4.0f, 0.1f, 0.5f));
	body->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//sphere->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
	AddChild(body);

	/*
	if(1){
		CubeModel* bodypart= new CubeModel();
		bodypart->SetScaling(vec3(0.7f, 0.3f, 2));
		bodypart->SetPosition(glm::vec3(-0.4f,	-0.1f,	-1.4f));
		bodypart->SetRotation(glm::vec3(0.0f,	1.0f,	0.0f), 5.0f);
		AddChild(bodypart);
	}
	if(1){
		CubeModel* bodypart= new CubeModel();
		bodypart->SetScaling(vec3(0.7f, 0.3f, 2));
		bodypart->SetPosition(glm::vec3(0.4f,	-0.1f,	-1.4f));
		bodypart->SetRotation(glm::vec3(0.0f,	1.0f,	0.0f), -5.0f);
		AddChild(bodypart);
	}
	//*/

	//Wing
	SphereModel* wing = new SphereModel();
	wing->SetScaling(glm::vec3(0.8f, 0.5f, 2.5f));
	wing->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//sphere->SetRotation(glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);
	AddChild(wing);


	//Rudder
	GroupModel* rudder = new GroupModel();
	glm::vec3 rudderSize = glm::vec3(0.07f,1.15f, 1.0f);
	if(1){
		GroupModel* rudderR = new GroupModel();
			CubeModel* r = new CubeModel();
			r->SetPosition(glm::vec3(0.0f,0.5f,-2.0f));
			r->SetRotation(glm::vec3(1.0f,0.0f,0.0f), -30.0f);	
			r->SetScaling(rudderSize);
		rudderR->AddChild(r);
		rudderR->SetRotation(glm::vec3(0,0,1), 45.0f);
		rudder->AddChild(rudderR);
	}
	if(1){
		GroupModel* rudderL = new GroupModel();
			CubeModel* b = new CubeModel();
			b->SetPosition(glm::vec3(0.0f,0.5f,-2.0f));
			b->SetRotation(glm::vec3(1.0f,0.0f,0.0f), -30.0f);	
			b->SetScaling(rudderSize);
		rudderL->AddChild(b);
		rudderL->SetRotation(glm::vec3(0,0,1), -45.0f);
		rudder->AddChild(rudderL);
	}
	if(1){
		GroupModel* rudderBL = new GroupModel();
			CubeModel* b = new CubeModel();
			b->SetPosition(glm::vec3(0.0f,-0.5f,-2.0f));
			b->SetRotation(glm::vec3(1.0f,0.0f,0.0f), -30.0f);	
			b->SetScaling(rudderSize);
		rudderBL->AddChild(b);
		//rudderBL->SetRotation(glm::vec3(0,0,1), -45.0f);
		rudder->AddChild(rudderBL);
	}
	rudder->SetRotation(glm::vec3(0.0f,0.0f,1.0f), 0.0f);
	AddChild(rudder);
	//*////////////////////////////////////////////////////
	//$#%^$#$^Y$T#%^&%$#W$^&$#%^&%$#%^&&%^$#$%^&%%$#$$^%#$%
	//Model Testing :: Transforms on multiple layers
	


	//--------------------------------------------------
	/* Rear rotating spheres
	if(true){
		GroupModel* propeller = new GroupModel();
		float bladeLength = 0.6f;
		int blades = 2;
		for(int i=0; i<blades;i++){	
			//Build blade
			GroupModel* blade = new GroupModel();
				SphereModel* sphere2 = new SphereModel();
				sphere2->SetScaling(glm::vec3(	0.2,	0.5f,	1.0f));
				sphere2->SetPosition(glm::vec3(	1.5f,	0.0f,	0.0f));
				sphere2->SetRotation(glm::vec3(	1.0f,	2.0f,	0.0f), 0.0f);
				blade->AddChild(sphere2);
			blade->SetRotation(glm::vec3(0.0f,0.0f,2.0f), i*(360.0f/blades));
			propeller->AddChild(blade);
		}
		propeller->SetPosition(glm::vec3(0.0f,0.0f,-2.5f));
		propeller->SetRotation(glm::vec3(0.0f,0.0f,1.0f), 0.0f);
		propeller->SetRotationSpeed(0.5f*360.0f);
		AddChild(propeller);
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
				sphere->SetScaling(glm::vec3(bladeLength, 0.06f, 0.1f));
				sphere->SetPosition(glm::vec3(bladeLength/2, 0.0f, 0.0f));
				sphere->SetRotation(glm::vec3(1.0f, 0.0f, 0.0f), 45.0f);
			blade->AddChild(sphere);
			blade->SetRotation(glm::vec3(0.0f,0.0f,2.0f), i*(360.0f/blades));
			propeller->AddChild(blade);
		}
	
		propeller->SetPosition(glm::vec3(0.0f,0.0f,2.5f));
		propeller->SetRotation(glm::vec3(0.0f,0.0f,1.0f), 0.0f);
		propeller->SetRotationSpeed(4.0f*360.0f);
		AddChild(propeller);
	}
	//*/

	


	/* Rear Thruster
	if(1){
		GroupModel* propeller2 = new GroupModel();
		int blades = 6;
		for(int i=0; i<blades;i++){	
			GroupModel* blade = new GroupModel();
				SphereModel* sphere2 = new SphereModel();
				sphere2->SetScaling(glm::vec3(	0.2,	0.7f,	0.5f)*0.8f);
				sphere2->SetPosition(glm::vec3(	0,	0.0f,	0.0f));
				sphere2->SetRotation(glm::vec3(	0.0f,	1.0f,	0.0f), -45.0f);
				blade->AddChild(sphere2);
			blade->SetRotation(glm::vec3(0.0f,0.0f,2.0f), i*(360.0f/blades));
			propeller2->AddChild(blade);
		}
		propeller2->SetPosition(glm::vec3(0.0f,0.0f,-2.3f));
		propeller2->SetRotation(glm::vec3(0.0f,0.0f,1.0f), 45.0f);
		propeller2->SetRotationSpeed(1.0f*360.0f);
		AddChild(propeller2);
	}

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
