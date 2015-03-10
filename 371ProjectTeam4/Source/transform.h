#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <GLM/glm.hpp>
#include "quaternion.h"
#include "mat4op.h"

class Transform
{
public:
	Transform(glm::vec3 position, quaternion rotation, glm::vec3 scale);
	Transform();

	void update(float dt);

	void setPosition(glm::vec3 v);
	void setRotation(quaternion q);
	void setScale(glm::vec3 v);

	mat4op getTransformation();
	glm::vec3 getPosition();
	quaternion getRotation();
	glm::vec3 getScale();
private:
	glm::vec3 m_position;
	quaternion m_rotation;
	glm::vec3 m_scale;
};

#endif
