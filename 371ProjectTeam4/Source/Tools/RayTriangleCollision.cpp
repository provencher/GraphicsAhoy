//
//  RayTriangleCollision.cpp
//  
//
//  Created by Nicholaos Mouzourakis on 3/19/15.
//
//

#include "RayTriangleCollision.h"

#include <GLM/glm.hpp>
#include <cmath>

bool RayTriangleCollision::TestIntersectionPlane(const glm::vec3& triangleA,
		const glm::vec3& triangleB, const glm::vec3& triangleC, const Ray& ray,
		glm::vec3& intersection, const bool shouldTestAsQuad = false)
{
	// Compute vectors
	glm::vec3 AC = triangleC - triangleA; //v0
	glm::vec3 AB = triangleB - triangleA; //v1
	Plane trianglePlane;
	trianglePlane.position = triangleA;
	trianglePlane.normal = glm::normalize(glm::cross(AB,AC));
	
	// Dot Product Between Plane Normal And Ray Direction
	double dotProduct = glm::dot(ray.getDirection(), trianglePlane.normal);
	
	// Determine If Ray Parallel To Plane
	if (dotProduct < 0.0f && dotProduct > -0.0f)
		return false;
	
	// Find Distance To Collision Point
	float distance = glm::dot(trianglePlane.normal, (trianglePlane.position - ray.getOrigin())) / dotProduct;
	
	if (distance < -0.0f) // Test If Collision Behind Start
		return false;
	
	intersection = ray.getOrigin() + ray.getDirection() * distance;
	
	glm::vec3 PA = intersection - triangleA; //v2
	
	// Compute dot products
	float dotAC2 = glm::dot(AC, (AC));
	float dotACAB = glm::dot(AC, (AB));
	float dotACPA = glm::dot(AC, (PA));
	float dotAB2 = glm::dot(AB, (AB));
	float dotABPA = glm::dot(AB, (PA));
	
	// Compute barycentric coordinates
	float invDenom = 1.0f / (dotAC2 * dotAB2 - dotACAB * dotACAB);
	float u = (dotAB2 * dotACPA - dotACAB * dotABPA) * invDenom;
	float v = (dotAC2 * dotABPA - dotACAB * dotACPA) * invDenom;
	
	// Check if point is in triangle/quad
	return (u >= 0.0f) && (v >= 0.0f) && (shouldTestAsQuad ? (u <= 1.0f && v <= 1.0f) : (u + v < 1.0f));
}