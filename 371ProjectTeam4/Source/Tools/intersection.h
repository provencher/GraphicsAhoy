#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <GLM/glm.hpp>
#include "material.h"

class Intersection
{
public:
	static Intersection invalidIntersection;

	Intersection(glm::vec3 position, glm::vec3 normal, float t, Material material);
	Intersection();

	void setPosition(glm::vec3 v);
	void setNormal(glm::vec3 v);
	void setT(float f);
	void setMaterial(Material m);

	glm::vec3 getPosition();
	glm::vec3 getNormal();
	float getT();
	Material getMaterial();

	inline bool operator==(const Intersection &x)
	{
		return m_position == x.m_position && m_normal == x.m_normal && m_t == x.m_t;
	}

	inline bool operator!=(const Intersection &x)
	{
		return !(*this == x);
	}
private:
	glm::vec3 m_position;
	glm::vec3 m_normal;
	float m_t;
	Material m_material;
protected:
};

#endif