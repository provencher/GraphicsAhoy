#include "plane.h"

Plane::Plane(glm::vec3 position, glm::vec3 normal, Material material)
{
	m_position = position;
	m_normal = normal;
	m_material = material;
}

Plane::Plane()
{
	m_position = glm::vec3();
	m_normal = glm::vec3(0, 1, 0);
	m_material = Material();
}

void Plane::setPosition(glm::vec3 v)
{
	m_position = v;
}

void Plane::setNormal(glm::vec3 v)
{
	m_normal = v;
}