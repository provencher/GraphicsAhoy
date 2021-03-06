//Created By Jordan Rutty

#include <GLM/glm.hpp>
#include <time.h>       
#include <iostream>

#include "ParticleEmitter.h"
#include "Particle.h"
#include "CubeModel.h"
#include "SphereModel.h"
#include "Billboard.h"
#include "../World.h"


//Emitter for Jet Thrusters
ParticleEmitter::ParticleEmitter(vec3 dir){
	Init(dir);

	mColor = vec3(0.9f,0.7f,0.0f);
}


void ParticleEmitter::Init(vec3 dir){
	mEmitting = false;
	mDirection = dir;

	//AddChild(new CubeModel(vec3(1), vec3(0.4)));
	

	SpawnParticle();
	mHost = this;
}

ParticleEmitter::~ParticleEmitter(){  

}




void ParticleEmitter::SpawnParticle(){
	float speed = (rand() % 10 + 50)/10;
	vec3 particleDir = mDirection+ vec3();
	float timestamp = 0;
	float life = 0.5f;
	float rotationSpeed = 0;

	Particle* p = new Particle(speed, particleDir, life, 0);
	vec3 size = vec3(0.5f, 0.3f, 0.3f);
	vec3 pos =  vec3(0,0,-0.1f);


	
	Model* m = new CubeModel(mColor, size, pos);
	p->SetRotationSpeed(180);
	p->SetPosition(0.0f*mDirection);
	p->AddChild("model", m);	//
	p->SetParent(this);			//draw model relative to emitter
	mParticles.push_back(p);	//add to special list

	/*note: add SetTarget to spawn particles relative to another element
	//convert between the two model spaces
	//= (inverse(target->GetWorldTransform())*emitter->GetWorldTransform())*positionToSpawn
	//vec3 pos = vec3(GetWorldMatrix()*vec4(mPosition,1)); //model to world space
	//*/

}

void ParticleEmitter::Update(float dt) {
	UpdateChildren(dt); // preserve nesting
	
	for (std::vector<Particle*>::iterator it = mParticles.begin(); it < mParticles.end();){
        if (*it != nullptr){
            (*it)->Update(dt);

            if (!(*it)->IsAlive()) {
				delete (*it);
                it = mParticles.erase(it);
            } else 
                it++;
        }
    }

	//Spawn Particles
	if(1 || rand() % 100 > 20){ // spawn every frame
		SpawnParticle();
	}
}

void ParticleEmitter::Draw() {
	DrawChildren(); // preserve nesting
	
	for (std::vector<Particle*>::iterator it = mParticles.begin(); it < mParticles.end();){
        if (*it != nullptr){
            (*it++)->Draw();
        }
    }
}
