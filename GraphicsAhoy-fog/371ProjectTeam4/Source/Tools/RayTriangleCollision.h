//
//  RayTriangleCollision.h
//  
//
//  Created by Nicholaos Mouzourakis on 3/19/15.
//
//

#ifndef ____RayTriangleCollision__
#define ____RayTriangleCollision__

#include "plane.h"

struct RayTriangleCollision
{
	//struct Vec3
	//{
	//	float x;
	//	float y;
	//	float z;
	//	
	//	float dot(const Vec3& other) const;
	//	Vec3 cross(const Vec3& other) const;
	//	Vec3 normalized() const;
	//	
	//	Vec3 operator* (const float multiplier) const;
	//	Vec3 operator/ (const float divider) const;
	//	Vec3 operator+ (const Vec3& adder) const;
	//	Vec3 operator- (const Vec3& subber) const;
	//};
	
	//struct Triangle
	//{
	//	Vec3 a;
	//	Vec3 b;
	//	Vec3 c;
	//};
	//
	//struct Ray
	//{
	//	Vec3 start;
	//	Vec3 direction;
	//};
	//
	//struct Plane
	//{
	//	Vec3 point;
	//	Vec3 normal;
	//};
	
	static bool TestIntersectionPlane(const glm::vec3& triangleA,
		const glm::vec3& triangleB, const glm::vec3& triangleC, const Ray& ray,
		const bool shouldTestAsQuad);
};

#endif /* defined(____RayTriangleCollision__) */
