#include "transform.h"

Transform::Transform(glm::vec3 position, quaternion rotation, glm::vec3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

Transform::Transform()
{
	m_position = glm::vec3();
	m_rotation = quaternion().identity();
	m_scale = glm::vec3();
}

void Transform::update(float dt)
{

}

void Transform::setPosition(glm::vec3 v)
{
	m_position = v;
}

void Transform::setRotation(quaternion q)
{
	m_rotation = q;
}

void Transform::setScale(glm::vec3 v)
{
	m_scale = v;
}

mat4op Transform::getTransformation()
{
	mat4op translation = mat4op().initTranslation(m_position.x, m_position.y, m_position.z);
	mat4op rotation = mat4op().initRotation(m_rotation.w, m_rotation.x, m_rotation.y, m_rotation.z);
	mat4op scale = mat4op().initScale(m_scale.x, m_scale.y, m_scale.z);

	return translation * rotation * scale;
}

glm::vec3 Transform::getPosition()
{
	return m_position;
}

quaternion Transform::getRotation()
{
	return m_rotation;
}

glm::vec3 Transform::getScale()
{
	return m_scale;
}

