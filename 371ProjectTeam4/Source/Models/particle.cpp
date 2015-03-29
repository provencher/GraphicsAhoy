

#include <GLM/glm.hpp>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>


#include "Particle.h"


Particle::Particle(float speed, vec3 dir, float life, float rotationSpeed){
	mSpeed = speed;
    mDirection = normalize(dir);
	mTime = 0.0f;
    mLifespan = life;
    mRotationSpeed = rotationSpeed;
	mAlive = true;
	mType = "Particle";


}
Particle::~Particle(){
    
}


void Particle::Update(float dt) {
    if(IsAlive()){


		vec3 deviation = vec3(
			(rand()%10-5)/5,
			(rand()%10-5)/5,
			(rand()%10-5)/5
		);

		mDirection += deviation*0.1f;
		SetPosition(GetPosition()+mSpeed*dt*(mDirection));

		if(mTime == 0.0f){
			mOriginalScale = GetScaling();
		} else {
			SetScaling(mOriginalScale*((mLifespan-mTime)/mLifespan));
		}
		mTime += dt;

		if(mTime >= mLifespan){
			mAlive = false;
		}
	}
	
	/*
	for (std::vector<Particle*>::iterator it = particles.begin(); it < particles.end();)
    {
        if (*it != nullptr){
            (*it)->Update(dt);
            if (!(*it)->isAlive()) {
                delete *it;
                it = particles.erase(it);
            }
            else
            {
                it++;
            }
        }
    }
	*/
}



void Particle::Draw(){
	DrawChildren();
}
