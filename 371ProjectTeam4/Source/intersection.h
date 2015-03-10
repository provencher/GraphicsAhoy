#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vec3.h"
#include "material.h"

class Intersection
{
public:
	static Intersection invalidIntersection;

	Intersection(vec3 position, vec3 normal, float t, Material material);
	Intersection();

	void setPosition(vec3 v);
	void setNormal(vec3 v);
	void setT(float f);
	void setMaterial(Material m);

	vec3 getPosition();
	vec3 getNormal();
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
	vec3 m_position;
	vec3 m_normal;
	float m_t;
	Material m_material;
protected:
};

#endif