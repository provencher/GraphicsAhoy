#ifndef RAY_H
#define RAY_H

#include <GLM/glm.hpp>

class Ray
{
public:
	Ray(glm::vec3 origin, glm::vec3 direction, float ior);
	Ray(glm::vec3 origin, glm::vec3 direction);
	Ray(glm::vec3 origin);
	Ray();

	void setOrigin(glm::vec3 v);
	void setDirection(glm::vec3 v);
	void setIOR(float f);
	glm::vec3 getOrigin() const;
	glm::vec3 getDirection() const;
	float getIOR();
private:
	glm::vec3 m_origin;
	glm::vec3 m_direction;
	float m_ior;
};

#endif