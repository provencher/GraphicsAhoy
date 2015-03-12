#ifndef LIGHT_H
#define LIGHT_H

#define LIGHT_NULL -1
#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1

#include "math2.h"
#include <GLM/glm.hpp>

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 direction, glm::vec3 color, glm::vec3 attenuation, float intensity);
	Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation, float intensity);
	Light(glm::vec3 position, glm::vec3 color, float intensity);
	Light();

	int getType()
	{
		if (m_direction != glm::vec3())
			return LIGHT_DIRECTIONAL;

		return LIGHT_POINT;
	}

	glm::vec3 getPosition();
	glm::vec3 getDirection();
	glm::vec3 getColor();
	glm::vec3 getAttenuation();
	float getIntensity();
private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_color;
	glm::vec3 m_attenuation;
	float m_intensity;
};

#endif