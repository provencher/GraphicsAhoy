#ifndef QUATERNION_H
#define QUATERNION_H

#define PI 3.14159265359f

#include <string>
#include <GLM/glm.hpp>

struct quaternion;
extern glm::vec3 operator*(glm::vec3 &v, quaternion &q);

struct quaternion
{
	float w, x, y, z;

	quaternion(float w, float x, float y, float z)
	{
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	quaternion()
	{
		w = 0.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	std::string toString()
	{
		return "quaternion[" + std::to_string(w) + " " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "]";
	}

	quaternion operator*(const quaternion &q)
	{
		quaternion result = quaternion();

		result.w = w * q.w - x * q.x - y * q.y - z * q.z;
		result.x = w * q.x + x * q.w + y * q.z - z * q.y;
		result.y = w * q.y - x * q.z + y * q.w + z * q.x;
		result.z = w * q.z + x * q.y - y * q.x + z * q.w;

		return result;
	}

	quaternion operator*(const glm::vec3 &v)
	{
		quaternion result = quaternion();

		result.w = -x * v.x - y * v.y - z * v.z;
		result.x = w * v.x + y * v.z - z * v.y;
		result.y = w * v.y + z * v.x - x * v.z;
		result.z = w * v.z + x * v.y - y * v.x;

		return result;
	}

	float length()
	{
		return std::sqrt(w * w + x * x + y * y + z * z);
	}

	quaternion normalize()
	{
		float l = length();
		return quaternion(w / l, x / l, y / l, z / l);
	}

	quaternion conjugate()
	{
		return quaternion(w, -x, -y, -z);
	}

	quaternion createFromAxisAngle(float &x, float &y, float &z, float theta)
	{
		theta = theta * PI / 180.0f;

		this->w = std::cos(theta / 2.0f);
		this->x = x * std::sin(theta / 2.0f);
		this->y = y * std::sin(theta / 2.0f);
		this->z = z * std::sin(theta / 2.0f);

		return *this;
	}

	quaternion identity()
	{
		w = 1.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;

		return *this;
	}

	glm::vec3 getForwardVector()
	{
		return glm::vec3(0, 0, 1) * *this;
	}

	glm::vec3 getUpVector()
	{
		return glm::vec3(0, 1, 0) * *this;
	}

	glm::vec3 getRightVector()
	{
		return glm::vec3(1, 0, 0) * *this;
	}
};

inline glm::vec3 operator*(glm::vec3 &v, quaternion &q)
{
	quaternion result = quaternion();
	result = q * v * q.conjugate();
	return glm::vec3(result.x, result.y, result.z);
}

#endif
