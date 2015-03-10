#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <algorithm>
#include <GLM/glm.hpp>

class math
{
public:
	static float clamp(float f, float min, float max)
	{
		return std::max(min, std::min(f, max));
	}

	static glm::vec3 clamp(glm::vec3 &v, float min, float max)
	{
		float x = clamp(v.x, min, max);
		float y = clamp(v.y, min, max);
		float z = clamp(v.z, min, max);

		return glm::vec3(x, y, z);
	}

	static float smoothstep(float f, float min, float max)
	{
		return clamp((f - min) / (max - min), 0.0f, 1.0f);
	}

	static glm::vec3 smoothstep(glm::vec3 &v, float min, float max)
	{
		float x = smoothstep(v.x, min, max);
		float y = smoothstep(v.y, min, max);
		float z = smoothstep(v.z, min, max);

		return glm::vec3(x, y, z);
	}

	static float interpolate(float f, float edgea, float edgeb)
	{
		return edgea + (edgeb - edgea) * clamp(f, 0.0f, 1.0f);
	}

	static glm::vec3 interpolate(glm::vec3 &v, float edgea, float edgeb)
	{
		float x = interpolate(v.x, edgea, edgeb);
		float y = interpolate(v.y, edgea, edgeb);
		float z = interpolate(v.z, edgea, edgeb);

		return glm::vec3(x, y, z);
	}
private:
	math();
protected:
};

#endif