

#include "GroupModel.h"

using namespace glm;

#pragma once
class Particle:public GroupModel
{

public:
    //Particle();
    Particle(float speed, vec3 dir, float life, float rotationSpeed);
    
    void Update(float dt);
	void Draw();

	bool IsAlive() {return mAlive;}
    ~Particle();

protected:
	bool mAlive;
    float mSpeed;
    vec3  mDirection;
	float mTime;
    float mLifespan;
    float mRotationSpeed;
	//-------------------
	vec3 mOriginalScale;
};

