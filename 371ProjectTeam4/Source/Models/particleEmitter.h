//Created By Jordan Rutty

#include <vector>
#include "Particle.h"
#include <GLM/glm.hpp>
#include <map>
using namespace glm;

#pragma once
class ParticleEmitter: public GroupModel
{
public:
    ParticleEmitter(vec3 dir=vec3(0,1,0));
	
    void Init(vec3 dir);

    ~ParticleEmitter();

    void Update(float dt);
    void Draw();
    
	void SpawnParticle();

	void StartEmitting(){mEmitting = true;}
	void StopEmitting(){mEmitting = false;}
	void IsActive();

    std::vector<Particle*> GetParticles();

protected:
	glm::vec3 mLastColor;
	std::vector<Particle*> mParticles;
	bool mEmitting;
	bool mAlive;
	glm::vec3 mColor;
	glm::vec3 mDirection;
	Model* mHost;

};

