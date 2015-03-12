#include "light.h"

Light::Light(glm::vec3 position, glm::vec3 direction, glm::vec3 color, glm::vec3 attenuation, float intensity)
{
	m_position = position;
	m_direction = glm::normalize(direction);
	m_color = color;
	m_attenuation = attenuation;
	m_intensity = intensity;
}

Light::Light(glm::vec3 position, glm::vec3 color, glm::vec3 attenuation, float intensity)
{
	m_position = position;
	m_direction = glm::vec3();
	m_color = color;
	m_attenuation = attenuation;
	m_intensity = intensity;
}

Light::Light(glm::vec3 position, glm::vec3 color, float intensity)
{
	m_position = position;	
	m_color = color;
	m_direction = glm::vec3();
	m_attenuation = glm::vec3();
	m_intensity = intensity;
}

Light::Light()
{
	m_position = glm::vec3();
	m_direction = glm::vec3();
	m_color = glm::vec3(1);
	m_attenuation = glm::vec3(0, 0, 1);
	m_intensity = 1;
}

glm::vec3 Light::getPosition()
{
	return m_position;
}

glm::vec3 Light::getDirection()
{
	return m_direction;
}

glm::vec3 Light::getColor()
{
	return m_color;
}

glm::vec3 Light::getAttenuation()
{
	return m_attenuation;
}

float Light::getIntensity()
{
	return m_intensity;
}