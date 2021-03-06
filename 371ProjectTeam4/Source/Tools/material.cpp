#include "material.h"

Material::Material(glm::vec3 emittance, glm::vec3 reflectance, float reflectivity, float refractivity, float ior)
{
	m_emittance = emittance;
	m_reflectance = reflectance;
	m_reflectivity = reflectivity;
	m_refractivity = refractivity;
	m_ior = ior;
}

Material::Material(glm::vec3 reflectance, float reflectivity, float refractivity, float ior)
{
	m_emittance = glm::vec3();
	m_reflectance = reflectance;
	m_reflectivity = reflectivity;
	m_refractivity = refractivity;
	m_ior = ior;
}

Material::Material()
{
	m_emittance = glm::vec3();
	m_reflectance = glm::vec3(1.0f);
	m_reflectivity = 0.0f;
	m_refractivity = 0.0f;
	m_ior = 1.0f;
}

glm::vec3 Material::getEmittance()
{
	return m_emittance;
}

glm::vec3 Material::getReflectance()
{
	return m_reflectance;
}

float Material::getReflectivity()
{
	return m_reflectivity;
}

float Material::getRefractivity()
{
	return m_refractivity;
}

float Material::getIOR()
{
	return m_ior;
}