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
#include "../Renderer.h"
#include "particleEmitter.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

PlaneModel::PlaneModel(vec3 size) : GroupModel(){

	color =  vec3(1,0,0);
	mBlack = 0.05f;
	//note if(1){} used to seperate scope to allow rapid development
	MakeBody();
	//MakePropeller();
	MakeWings();
	MakeGuns();
	MakeRudder();
	MakeThrusters();
	MakeThrust();
	

}

PlaneModel::~PlaneModel(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}


void PlaneModel::MakePropeller(){


	//########################################################################################

	
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
}

void PlaneModel::MakeBody(){

	//########################################################################################
	
	if(1){//body
		SphereModel* model = new SphereModel(color);
		model->SetScaling(vec3(0.8f, 0.5f, 2.5f));
		model->SetPosition(vec3(0.0f, 0.0f, 0.0f));
		//model->SetRotation(vec3(0.0f, 1.0f, 0.0f), 45.0f);
		AddChild("body", model);
	}

	//########################################################################################

	if(1){//cockpit
		GroupModel* group = new GroupModel();
		if(1){ //glass
			float col = mBlack;
			SphereModel* model = new SphereModel(vec3(col,col,col));
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

	

}
void PlaneModel::MakeWings(){
//########################################################################################
	//wing
	GroupModel* wing = new GroupModel();
	SphereModel* wingbase = new SphereModel();
	wingbase->SetScaling(vec3(4.0f, 0.1f, 0.5f));
	wingbase->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	//sphere->SetRotation(vec3(0.0f, 1.0f, 0.0f), 45.0f);
	wing->AddChild("wing_base", wingbase);
	AddChild("wing", wing);
	//Flaps ---------------------------------------------------------------------------------
	float flapWidth = 0.45f;// z width of flap
	float flapLen = 2.0f;//  3.4 full wing
	float wingAng = 6; // rotate to fit wing
	float zOffset = -0.2;//-0.2;
	float xOffset = 1.78f;//2.3f;
	vec3 flapColor = vec3(0.4f, 0,0);
	//Left Flap -----------------------------------------------------------------------------
	if(1){
		GroupModel* wingside = new GroupModel();					//wingside->AddChild(new CubeModel(vec3(0,0,1), vec3(0.2f))); //marker
			if(1){
				Model* m = new CubeModel(flapColor);	//flap->AddChild(new CubeModel(vec3(1), vec3(0.2f))); //marker
				m->SetPosition(vec3(1.60f,-0.025,-0.42f));
				m->SetScaling(vec3(1.2f,0.05f, flapWidth));
				wingside->AddChild(m);
			}
		
			GroupModel* flap = new GroupModel();
				//flaps cube
				Model* m = new CubeModel(flapColor);	//flap->AddChild(new CubeModel(vec3(1), vec3(0.2f))); //marker
				m->SetPosition(vec3(0,-0.025,-flapWidth/2));
				m->SetScaling(vec3(flapLen,0.05f, flapWidth));
			
			flap->SetRotation(vec3(1,0,0), 0); // angle of flap
			flap->SetPosition(vec3(0,0,zOffset));
			//flap->SetRotationSpeed(60);
			flap->AddChild("flap", m);
		
		wingside->SetRotation(vec3(0,1,0), -wingAng);
		wingside->SetPosition(vec3(xOffset,0,0));
		wingside->AddChild("flap",flap);
		wing->AddChild("left", wingside);
	}
	//Right Flap -----------------------------------------------------------------------------
	if(1){
		GroupModel* wingside = new GroupModel();					//wingside->AddChild(new CubeModel(vec3(0,0,1), vec3(0.2f))); //marker
			if(1){
				Model* m = new CubeModel(flapColor);	//flap->AddChild(new CubeModel(vec3(1), vec3(0.2f))); //marker
				m->SetPosition(vec3(-1.60f,-0.025,-0.42f));
				m->SetScaling(vec3(1.2f,0.05f, flapWidth));
				wingside->AddChild(m);
			}
		
			GroupModel* flap = new GroupModel();
				//flaps cube
				Model* m = new CubeModel(flapColor);	//flap->AddChild(new CubeModel(vec3(1), vec3(0.2f))); //marker
				m->SetPosition(vec3(0,-0.025,-flapWidth/2));
				m->SetScaling(vec3(flapLen,0.05f, flapWidth));
			
			flap->SetRotation(vec3(1,0,0), 0); // angle of flap
			flap->SetPosition(vec3(0,0,zOffset));
			//flap->SetRotationSpeed(60);
			flap->AddChild("flap", m);
		
		wingside->SetRotation(vec3(0,1,0), wingAng);
		wingside->SetPosition(vec3(-xOffset,0,0));
		wingside->AddChild("flap",flap);
		wing->AddChild("right", wingside);
	}
	
	//########################################################################################
}
void PlaneModel::MakeGuns(){
	
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
				Model* m = new CubeModel(vec3(mBlack));
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
				Model* m = new CubeModel(vec3(mBlack));
				m->SetPosition(vec3(0,0,0.0f));
				m->SetScaling(boxsize);
				g->AddChild(m);
			}
			//g->SetScaling(vec3(2));
			g->SetPosition(vec3(1.7f,0.2f,0.0f));
			torrentLayer->AddChild(g);
		}
	
		torrentLayer->SetPosition(vec3(0,-0.2f,0.22));
		AddChild("guns", torrentLayer);
	}
}
void PlaneModel::MakeThrusters(){

	//########################################################################################

	//*
	if(1){
		vec3 thrusterColor = vec3(mBlack,mBlack,mBlack);
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
	}

}
void PlaneModel::MakeThrust(){

	//########################################################################################

	//Left afterburner
	if(1){
		ParticleEmitter* pe = new ParticleEmitter(vec3(0,0,-1));
		pe->SetPosition(vec3(-0.5f,-0.1f,-2.4));
		AddChild("left-afterburner", pe);
	}

	//Right afterburner
	if(1){
		ParticleEmitter* pe = new ParticleEmitter(vec3(0,0,-1));
		pe->SetPosition(vec3(0.5f,-0.1f,-2.4));
		AddChild("right-afterburner", pe);
	}
	

	//Light ---------------------------------------------
	if(1){
		mLightModel = new LightModel();
		AddChild(mLightModel);
		mLightModel->SetPosition(vec3(0,0.5f,-2.5f));
		mLightModel->SetIntensities(4.2f*vec3(1,0.6f,0.4f));
		mLightModel->SetIsDirectional(1);
		mLightModel->SetAttenuation(0.05f);
		mLightModel->SetAmbientCoefficient(0.0f);
	}


	//Cast Darkness
	if(1){
		LightModel* lm = new LightModel();
		lm->SetPosition(vec3(0,0.0f,-1.5f));
		lm->SetIntensities(-1.2f*vec3(1,0.6f,0.4f));
		lm->SetIsDirectional(1);
		lm->SetAttenuation(0.2f);
		lm->SetAmbientCoefficient(0.0f);
		//mLightModel->SetConeAngle(45);
		//mLightModel->SetConeDirection(vec3(0,0,-1));//relative to world
		AddChild(lm);
	}



	//*/
}
void PlaneModel::MakeRudder(){
	


	//########################################################################################

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
	rudder->SetRotation(vec3(0.0f,0.0f,1.0f), 0.0f);
	AddChild("rudder",rudder);
	//*////////////////////////////////////////////////////
	//$#%^$#$^Y$T#%^&%$#W$^&$#%^&%$#%^&&%^$#$%^&%%$#$$^%#$%
	//Model Testing :: Transforms on multiple layers
	


	//########################################################################################
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
