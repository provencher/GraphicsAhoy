#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material
{
public:
	Material(vec3 emittance, vec3 reflectance, float reflectivity, float refractivity, float ior);
	Material(vec3 reflectance, float reflectivity, float refractivity, float ior);
	Material();

	vec3 getEmittance();
	vec3 getReflectance();
	float getReflectivity();
	float getRefractivity();
	float getIOR();
private:
	vec3 m_emittance;
	vec3 m_reflectance;
	float m_reflectivity;
	float m_refractivity;
	float m_ior;
};

#endif