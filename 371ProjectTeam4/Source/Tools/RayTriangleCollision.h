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
	static bool TestIntersectionPlane(const glm::vec3& triangleA,
		const glm::vec3& triangleB, const glm::vec3& triangleC, const Ray& ray,
		glm::vec3& intersection, const bool shouldTestAsQuad);
};

#endif /* defined(____RayTriangleCollision__) */
