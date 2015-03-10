#ifndef MATERIAL_H
#define MATERIAL_H

#include <GLM/glm.hpp>

class Material
{
public:
	Material(glm::vec3 emittance, glm::vec3 reflectance, float reflectivity, float refractivity, float ior);
	Material(glm::vec3 reflectance, float reflectivity, float refractivity, float ior);
	Material();

	glm::vec3 getEmittance();
	glm::vec3 getReflectance();
	float getReflectivity();
	float getRefractivity();
	float getIOR();
private:
	glm::vec3 m_emittance;
	glm::vec3 m_reflectance;
	float m_reflectivity;
	float m_refractivity;
	float m_ior;
};

#endif