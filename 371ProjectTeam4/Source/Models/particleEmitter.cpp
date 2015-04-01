

#include <GLM/glm.hpp>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>

#include "ParticleEmitter.h"
#include "Particle.h"
#include "CubeModel.h"
#include "SphereModel.h"
#include "Billboard.h"
#include "../World.h"

ParticleEmitter::ParticleEmitter(){
	Init(vec3(0,0,-1));

	mColor = vec3(0.1f,0.7f,1.0f);
}

ParticleEmitter::ParticleEmitter(vec3 dir){
	Init(dir);
}

void ParticleEmitter::Init(vec3 dir){
	mEmitting = false;
	mDirection = dir;

	if(0){
		Model* m = new CubeModel(vec3(1), vec3(0.4));
		AddChild(m);
	}

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
	vec3 size = vec3(0.5f, 0.3f, 0.2f);
	vec3 pos =  vec3(0,0,-0.1f);


	vec3 col = vec3(
					((rand()%10)/10.0f),
					((rand()%10)/10.0f),
					((rand()%10)/10.0f)
				);
	Model* m = new CubeModel(col, size, pos);
	mLastColor = col;
	//BillBoard* m = new BillBoard();
	//World* w = World::GetInstance();
	//m->SetLookAtCamera(w->GetCamera());


	//note: add SetTarget to spawn particles relative to
	//= (inverse(GetWorldTransform())*target->GetWorldTransform()) *positionToSpawn

	p->SetRotationSpeed(180);
	p->SetPosition(0.0f*mDirection);
	p->AddChild(m);
	p->SetParent(this);
	//AddChild("test", p);
	mParticles.push_back(p);
}

void ParticleEmitter::Update(float dt) {
	if(!mParticles.empty()){
	//	std::cout << "parrticle exists\n";
	}
	//UpdateChildren(dt);
	

	for (std::vector<Particle*>::iterator it = mParticles.begin(); it < mParticles.end();){
        if (*it != nullptr){
            (*it)->Update(dt);

            if (!(*it)->IsAlive()) {
                delete *it;
                it = mParticles.erase(it);
            } else 
                it++;
        }
    }

	if(1||rand() % 100 > 20){
		SpawnParticle();
	}
}

void ParticleEmitter::Draw() {
	DrawChildren();
	
	for (std::vector<Particle*>::iterator it = mParticles.begin(); it < mParticles.end();){
        if (*it != nullptr){
            (*it++)->Draw();
        }
    }
}
