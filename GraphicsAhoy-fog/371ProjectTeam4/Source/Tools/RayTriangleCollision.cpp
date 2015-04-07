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

//float RayTriangleCollision::Vec3::dot(const RayTriangleCollision::Vec3& other) const
//{
//	return this->x * other.x + this->y * other.y + this->z * other.z;
//}
//
//RayTriangleCollision::Vec3 RayTriangleCollision::Vec3::cross(const RayTriangleCollision::Vec3& other) const
//{
//	RayTriangleCollision::Vec3 result;
//	result.x = this->y * other.z - other.y * this->z;
//	result.y = this->z * other.x - other.z * this->x;
//	result.z = this->x * other.y - other.x * this->y;
//	return result;
//}
//
//RayTriangleCollision::Vec3 RayTriangleCollision::Vec3::normalized() const
//{
//	return *this / sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
//}
//
//RayTriangleCollision::Vec3 RayTriangleCollision::Vec3::operator* (const float multiplier) const
//{
//	RayTriangleCollision::Vec3 result = *this;
//	result.x *= multiplier;
//	result.y *= multiplier;
//	result.z *= multiplier;
//	return result;
//}
//
//RayTriangleCollision::Vec3 RayTriangleCollision::Vec3::operator/ (const float divider) const
//{
//	RayTriangleCollision::Vec3 result = *this;
//	result.x /= divider;
//	result.y /= divider;
//	result.z /= divider;
//	return result;
//}
//
//RayTriangleCollision::Vec3 RayTriangleCollision::Vec3::operator+ (const RayTriangleCollision::Vec3& adder) const
//{
//	 result = *this;
//	result.x += adder.x;
//	result.y += adder.y;
//	result.z += adder.z;
//	return result;
//}
//
//RayTriangleCollision::Vec3 RayTriangleCollision::Vec3::operator- (const RayTriangleCollision::Vec3& subber) const
//{
//	RayTriangleCollision::Vec3 result = *this;
//	result.x -= subber.x;
//	result.y -= subber.y;
//	result.z -= subber.z;
//	return result;
//}

bool RayTriangleCollision::TestIntersectionPlane(const glm::vec3& triangleA,
		const glm::vec3& triangleB, const glm::vec3& triangleC, const Ray& ray,
		const bool shouldTestAsQuad = false)
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
	
	glm::vec3 intersection = ray.getOrigin() + ray.getDirection() * distance;
	
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

//int main()
//{
//	RayTriangleCollision::Triangle triangle;
//	RayTriangleCollision::Ray ray;
//	
//	triangle.a.x = 0.0f;
//	triangle.a.y = 0.0f;
//	triangle.a.z = 0.0f;
//	
//	triangle.b.x = 2.0f;
//	triangle.b.y = 0.0f;
//	triangle.b.z = 0.0f;
//	
//	triangle.c.x = 3.0f;
//	triangle.c.y = 2.0f;
//	triangle.c.z = 0.0f;
//	
//	ray.start.x = 1.0f;
//	ray.start.y = 1.0f;
//	ray.start.z = 1.0f;
//	
//	ray.direction.x = 0.0f;
//	ray.direction.y = 0.0f;
//	ray.direction.z = -1.0f;
//	
//	std::cout << RayTriangleCollision::TestIntersectionPlane(triangle, ray) << std::endl;
//	
//	return 0;
//}


