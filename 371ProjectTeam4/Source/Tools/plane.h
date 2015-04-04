#ifndef PLANE_H
#define PLANE_H

#include <GLM/glm.hpp>
#include "ray.h"
#include "intersection.h"
#include "material.h"

class Plane
{
public:
	Plane(glm::vec3 position, glm::vec3 normal, Material material);
	Plane();

	Intersection intersect(Ray &r)
	{
		glm::vec3 P;
		float d, t;

		P = position - r.getOrigin();
		d = glm::dot(normal, r.getDirection());

		if (d > 0.0f)
			return Intersection::invalidIntersection;

		t = glm::dot(P, normal) / d;

		if (t < 1e-3)
			return Intersection::invalidIntersection;

		Intersection x = Intersection();
		x.setPosition(r.getOrigin() + r.getDirection() * t);
		x.setNormal(normal);
		x.setT(t);
		x.setMaterial(m_material);

		return x;
	}
	
	glm::vec3 position;
	glm::vec3 normal;
private:
	Material m_material;
protected:
};

#endif