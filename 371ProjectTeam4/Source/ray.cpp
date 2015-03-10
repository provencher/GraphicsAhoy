#include "ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction, float ior)
{
	m_origin = origin;
	m_direction = glm::normalize(direction);
	m_ior = ior;
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
{
	m_origin = origin;
	m_direction = glm::normalize(direction);
	m_ior = 1.0f;
}

Ray::Ray(glm::vec3 origin)
{
	m_origin = origin;
	m_direction = glm::vec3();
	m_ior = 1.0f;
}

Ray::Ray()
{
	m_origin = glm::vec3();
	m_direction = glm::vec3();
	m_ior = 1.0f;
}

void Ray::setOrigin(glm::vec3 v)
{
	m_origin = v;
}

void Ray::setDirection(glm::vec3 v)
{
	m_direction = glm::normalize(v);
}

void Ray::setIOR(float f)
{
	m_ior = f;
}

glm::vec3 Ray::getOrigin()
{
	return m_origin;
}

glm::vec3 Ray::getDirection()
{
	return m_direction;
}

float Ray::getIOR()
{
	return m_ior;
}