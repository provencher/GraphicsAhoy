#include "plane.h"

Plane::Plane(glm::vec3 position, glm::vec3 normal, Material material)
{
	position = position;
	normal = normal;
	m_material = material;
}

Plane::Plane()
{
	position = glm::vec3();
	normal = glm::vec3(0, 1, 0);
	m_material = Material();
}