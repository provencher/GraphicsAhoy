#include "intersection.h"

Intersection Intersection::invalidIntersection = Intersection(glm::vec3(), glm::vec3(), -1.0f, Material());

Intersection::Intersection(glm::vec3 position, glm::vec3 normal, float t, Material material)
{
	m_position = position;
	m_normal = normal;
	m_t = t;
	m_material = material;
}

Intersection::Intersection()
{
	m_position = glm::vec3();
	m_normal = glm::vec3();
	m_t = 0.0f;
	m_material = Material();
}

void Intersection::setPosition(glm::vec3 v)
{
	m_position = v;
}

void Intersection::setNormal(glm::vec3 v)
{
	m_normal = v;
}

void Intersection::setT(float f)
{
	m_t = f;
}

void Intersection::setMaterial(Material m)
{
	m_material = m;
}

glm::vec3 Intersection::getPosition()
{
	return m_position;
}

glm::vec3 Intersection::getNormal()
{
	return m_normal;
}

float Intersection::getT()
{
	return m_t;
}

Material Intersection::getMaterial()
{
	return m_material;
}